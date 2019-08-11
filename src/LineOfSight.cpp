#include <math.h>
#include "..\include\LOSTable.h"
#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  This function computes the total loss in the line-of
 |                sight region as described in Annex 2, Section 6 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  path          - Struct containing path parameters
 |                terminal_1    - Struct containing low terminal parameters
 |                terminal_2    - Struct containing high terminal parameters
 |                f__mhz        - Frequency, in MHz
 |                A_dML__db     - Diffraction loss at d_ML, in dB
 |                q             - Quantile
 |                d__km         - Path length, in km
 |
 |      Outputs:  los_params    - Struct contianing LOS parameters
 |                result        - Struct contianing P.528 results
 |                K_LOS         - K-value
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void LineOfSight(Path *path, Terminal terminal_1, Terminal terminal_2, LineOfSightParams *los_params, double f__mhz, double A_dML__db,
    double q, double d__km, Result *result, double *K_LOS)
{
    double psi;
    double R_Tg;

    // 0.2997925 = speed of light, megameters per sec
    double lambda = 0.2997925 / f__mhz;

    // Build table of psi, delta_r, and d__km relationships
    LOSTable table;
    table.Build(*path, terminal_1, terminal_2, lambda);

    // determine psi_limit, where you switch from free space to 2-ray model
    // lambda / 2 is the start of the lobe closest to d_ML
    double d__km_s = table.GetDistanceFromTable(lambda / 2);

    double psi_limit = table.GetPsiFromTable(d__km_s);

    /////////////////////////////////////////////
    // Determine d_0__km distance
    //

    // "[d_y6__km] is the largest distance at which a free-space value is obtained in a two-ray model
    //   of reflection from a smooth earth with a reflection coefficient of -1" [ES-83-3, page 44]
    double d_y6__km = table.GetDistanceFromTable(lambda / 6);

    // In IF-73, the values for d_0 (d_d in IF-77) were found to be too small when both antennas are low,
    // so this "heuristic" was developed to fix that
    // [ES-83-3, Eqn 172]
    if (terminal_1.d__km >= path->d_d__km || path->d_d__km >= path->d_ML__km)
    {
        if (terminal_1.d__km > d_y6__km || d_y6__km > path->d_ML__km)
            path->d_0__km = terminal_1.d__km;
        else
            path->d_0__km = d_y6__km;
    }
    else if (path->d_d__km < d_y6__km && d_y6__km < path->d_ML__km)
        path->d_0__km = d_y6__km;
    else
        path->d_0__km = path->d_d__km;

    //
    // Determine d_0__km distance
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // Tune d_0__km distance
    //

    // Now that we have d_0, lets carefully walk it forward, 1 meter at a time, to tune it to as precise as possible without
    //      going beyond the LOS region (ie, beyond d_ML)
    double d_temp__km = path->d_0__km;
    while (true)
    {
        psi = table.GetPsiFromTable(d_temp__km);

        LineOfSightParams result;
        RayOptics(*path, terminal_1, terminal_2, psi, &result);

        // if the resulting distance is beyond d_0 OR if we incremented again we'd be outside of LOS...
        if (result.d__km >= path->d_0__km || (d_temp__km + 0.001) >= path->d_ML__km)
        {
            // use the resulting distance as d_0
            path->d_0__km = result.d__km;
            break;
        }

        d_temp__km += 0.001;
    }

    //
    // Tune d_0__km distance
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // Compute loss at d_0__km
    //

    double psi_d0 = table.GetPsiFromTable(path->d_0__km);
    RayOptics(*path, terminal_1, terminal_2, psi_d0, los_params);

    GetPathLoss(psi_d0, *path, terminal_1, terminal_2, f__mhz, psi_limit, A_dML__db, 0, los_params, &R_Tg);

    //
    // Compute loss at d_0__km
    /////////////////////////////////////////////

    // tune psi for the desired distance
    psi = table.GetPsiFromTable(d__km);

    if (d__km != 0 && psi != 0)
    {
        double delta = 0.01;
        int los_iterations = 0;

        while (los_iterations < LOS_ITERATION)
        {
            RayOptics(*path, terminal_1, terminal_2, psi, los_params);

            if (d__km - los_params->d__km < LOS_EPSILON && d__km > los_params->d__km)
                break;
            else if (los_params->d__km < d__km)
                psi -= delta;
            else
            {
                psi += delta;
                delta /= 2;
                psi -= delta;
            }

            los_iterations++;
        }
    }
    else
        RayOptics(*path, terminal_1, terminal_2, psi, los_params);

    GetPathLoss(psi, *path, terminal_1, terminal_2, f__mhz, psi_limit, A_dML__db, los_params->A_LOS__db, los_params, &R_Tg);

    /////////////////////////////////////////////
    // Compute atmospheric absorption
    //

    double r_eo__km = CalculateEffectiveRayLength(los_params->z__km[0], los_params->z__km[1], los_params->a_a__km, los_params->r_0__km, los_params->theta_h1, T_eo__km);
    double r_ew__km = CalculateEffectiveRayLength(los_params->z__km[0], los_params->z__km[1], los_params->a_a__km, los_params->r_0__km, los_params->theta_h1, T_ew__km);

    double gamma_oo, gamma_ow;
    AtmosphericAbsorptionParameters(f__mhz, &gamma_oo, &gamma_ow);

    double A_a__db = -gamma_oo * r_eo__km - gamma_ow * r_ew__km;

    //
    // Compute atmospheric absorption
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // Compute free-space loss
    //

    double theta_fs = ((los_params->theta[0] + los_params->theta[1]) * los_params->a_a__km) / a_0__km;

    double z_1__km = a_0__km + terminal_1.h_r__km;
    double z_2__km = a_0__km + terminal_2.h_r__km;
    double r_FS__km = MAX(sqrt(pow(z_2__km - z_1__km, 2) + (4.0 * z_1__km * z_2__km * pow(sin(theta_fs * 0.5), 2))), fabs(z_2__km - z_1__km));   // Total ray length for free space path loss

    double L_bf__db = -32.45 - 20.0 * log10(f__mhz);
    result->A_fs__db = L_bf__db - 20.0 * log10(r_FS__km);

    //
    // Compute free-space loss
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // Compute variability
    //

    double f_theta_h;
    if (los_params->theta_h1 <= 0.0)
        f_theta_h = 1.0;
    else if (los_params->theta_h1 >= 1.0)
        f_theta_h = 0.0;
    else
        f_theta_h = MAX(0.5 - 0.3183098862 * (atan(20.0 * log10(32.0 * los_params->theta_h1))), 0);

    double Y_e__db, Y_e_50__db, A_Y;
    LongTermVariability(terminal_1.h_r__km, terminal_2.h_r__km, d__km, f__mhz, q, f_theta_h, los_params->A_LOS__db, &Y_e__db, &A_Y);
    LongTermVariability(terminal_1.h_r__km, terminal_2.h_r__km, d__km, f__mhz, 0.50, f_theta_h, los_params->A_LOS__db, &Y_e_50__db, &A_Y);

    double F_AY;
    if (A_Y <= 0.0)
        F_AY = 1.0;
    else if (A_Y >= 9.0)
        F_AY = 0.1;
    else
        F_AY = (1.1 + (0.9 * cos((A_Y / 9.0) * PI))) / 2.0;

    double F_delta_r;
    if (los_params->delta_r >= (lambda / 2.0))
        F_delta_r = 1.0;
    else if (los_params->delta_r <= lambda / 6.0)
        F_delta_r = 0.1;
    else
        F_delta_r = 0.5 * (1.1 - (0.9 * cos(((3.0 * PI) / lambda) * (los_params->delta_r - (lambda / 6.0)))));

    double R_s = R_Tg * F_delta_r * F_AY;

    double W_a;
    if (r_ew__km <= 0.0)
        W_a = 0.0001;
    else
    {
        double Y_pi_99__db = 10.0 * log10(f__mhz * pow(r_ew__km, 3)) - 84.26;
        double K_t = FindKForYpiAt99Percent(Y_pi_99__db);

        W_a = pow(10.0, K_t / 10.0);
    }

    double W_R = pow(R_s, 2) + pow(0.01, 2);

    double W = W_R + W_a;

    if (W <= 0.0)
        *K_LOS = -40.0;
    else
    {
        *K_LOS = 10.0 * log10(W);

        if (*K_LOS < -40.0)
            *K_LOS = -40.0;
    }

    double Y_pi_50__db = 0.0;   //  zero mean
    double Y_pi__db = NakagamiRice(*K_LOS, q);

    double Y_total__db = CombineDistributions(Y_e_50__db, Y_e__db, Y_pi_50__db, Y_pi__db, q);

    //
    // Compute variability
    /////////////////////////////////////////////

    result->d__km = los_params->d__km;
    result->A__db = result->A_fs__db + A_a__db + los_params->A_LOS__db + Y_total__db;
}
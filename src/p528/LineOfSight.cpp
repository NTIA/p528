#include "../../include/p528.h"
#include "../../include/p676.h"

double FindPsiAtDistance(double d__km, Path *path, Terminal *terminal_1, Terminal *terminal_2)
{
    if (d__km == 0)
        return PI / 2;

    // initialize to start at mid-point
    double psi = PI / 2; 
    double delta_psi = -PI / 4;

    double d_psi__km;

    do
    {
        psi += delta_psi; // new psi

        LineOfSightParams params_temp;
        RayOptics(terminal_1, terminal_2, psi, &params_temp);

        d_psi__km = params_temp.d__km;

        // compute delta
        if (d_psi__km > d__km)
            delta_psi = abs(delta_psi) / 2;
        else
            delta_psi = -abs(delta_psi) / 2;

    } while (abs(d__km - d_psi__km) > 1e-3 && (abs(delta_psi) > 1e-12));  // get within 1 meter of desired delta_r value

    return psi;
}

double FindPsiAtDeltaR(double delta_r__km, Path *path, Terminal *terminal_1, Terminal *terminal_2, double terminate)
{
    double psi = PI / 2;
    double delta_psi = -PI / 4;

    LineOfSightParams params_temp;
    do
    {
        psi += delta_psi;

        RayOptics(terminal_1, terminal_2, psi, &params_temp);

        if (params_temp.delta_r__km > delta_r__km)
            delta_psi = -abs(delta_psi) / 2;
        else
            delta_psi = abs(delta_psi) / 2;

    } while (abs(params_temp.delta_r__km - delta_r__km) > terminate);

    return psi;
}

double FindDistanceAtDeltaR(double delta_r__km, Path *path, Terminal *terminal_1, Terminal *terminal_2, double terminate)
{
    double psi = PI / 2;
    double delta_psi = -PI / 4;

    LineOfSightParams params_temp;
    do
    {
        psi += delta_psi;

        RayOptics(terminal_1, terminal_2, psi, &params_temp);

        if (params_temp.delta_r__km > delta_r__km)
            delta_psi = -abs(delta_psi) / 2;
        else
            delta_psi = abs(delta_psi) / 2;

    } while (abs(params_temp.delta_r__km - delta_r__km) > terminate);

    return params_temp.d__km;
}

/*=============================================================================
 |
 |  Description:  This function computes the total loss in the line-of-sight
 |                region as described in Annex 2, Section 6 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  path          - Struct containing path parameters
 |                terminal_1    - Struct containing low terminal parameters
 |                terminal_2    - Struct containing high terminal parameters
 |                f__mhz        - Frequency, in MHz
 |                A_dML__db     - Diffraction loss at d_ML, in dB
 |                p             - Time percentage
 |                d__km         - Path length, in km
 |                T_pol         - Code indicating either polarization
 |                                  + 0 : POLARIZATION__HORIZONTAL
 |                                  + 1 : POLARIZATION__VERTICAL
 |
 |      Outputs:  los_params    - Struct containing LOS parameters
 |                result        - Struct containing P.528 results
 |                K_LOS         - K-value
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void LineOfSight(Path *path, Terminal *terminal_1, Terminal *terminal_2, LineOfSightParams *los_params, 
    double f__mhz, double A_dML__db, double p, double d__km, int T_pol, Result *result, double *K_LOS)
{
    double psi;
    double R_Tg;

    // 0.2997925 = speed of light, gigameters per sec
    double lambda__km = 0.2997925 / f__mhz;                             // [Eqn 6-1]
    double terminate = lambda__km / 1e6;

    // determine psi_limit, where you switch from free space to 2-ray model
    // lambda / 2 is the start of the lobe closest to d_ML
    double psi_limit = FindPsiAtDeltaR(lambda__km / 2, path, terminal_1, terminal_2, terminate);

    // "[d_y6__km] is the largest distance at which a free-space value is obtained in a two-ray model
    //   of reflection from a smooth earth with a reflection coefficient of -1" [ES-83-3, page 44]
    double d_y6__km = FindDistanceAtDeltaR(lambda__km / 6, path, terminal_1, terminal_2, terminate);

    /////////////////////////////////////////////
    // Determine d_0__km distance
    //

    // In IF-73, the values for d_0 (d_d in IF-77) were found to be too small when both antennas are low,
    // so this "heuristic" was developed to fix that
    // [Eqns 8-2 and 8-3]
    if (terminal_1->d_r__km >= path->d_d__km || path->d_d__km >= path->d_ML__km)
    {
        if (terminal_1->d_r__km > d_y6__km || d_y6__km > path->d_ML__km)
            path->d_0__km = terminal_1->d_r__km;
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
        psi = FindPsiAtDistance(d_temp__km, path, terminal_1, terminal_2);

        LineOfSightParams los_result;
        RayOptics(terminal_1, terminal_2, psi, &los_result);

        // if the resulting distance is beyond d_0 OR if we incremented again we'd be outside of LOS...
        if (los_result.d__km >= path->d_0__km || (d_temp__km + 0.001) >= path->d_ML__km)
        {
            // use the resulting distance as d_0
            path->d_0__km = los_result.d__km;
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

    double psi_d0 = FindPsiAtDistance(path->d_0__km, path, terminal_1, terminal_2);

    RayOptics(terminal_1, terminal_2, psi_d0, los_params);

    GetPathLoss(psi_d0, path, f__mhz, psi_limit, A_dML__db, 0, T_pol, los_params, &R_Tg);

    //
    // Compute loss at d_0__km
    /////////////////////////////////////////////

    // tune psi for the desired distance
    psi = FindPsiAtDistance(d__km, path, terminal_1, terminal_2);

    RayOptics(terminal_1, terminal_2, psi, los_params);

    GetPathLoss(psi, path, f__mhz, psi_limit, A_dML__db, los_params->A_LOS__db, T_pol, los_params, &R_Tg);

    /////////////////////////////////////////////
    // Compute atmospheric absorption
    //

    SlantPathAttenuationResult result_slant;
    SlantPathAttenuation(f__mhz / 1000, terminal_1->h_r__km, terminal_2->h_r__km, PI / 2 - los_params->theta_h1__rad, &result_slant);

    result->A_a__db = result_slant.A_gas__db;

    //
    // Compute atmospheric absorption
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // Compute free-space loss
    //

    result->A_fs__db = 20.0 * log10(los_params->r_0__km) + 20.0 * log10(f__mhz) + 32.45; // [Eqn 6-4]

    //
    // Compute free-space loss
    /////////////////////////////////////////////

    /////////////////////////////////////////////
    // Compute variability
    //

    // [Eqn 13-1]
    double f_theta_h;
    if (los_params->theta_h1__rad <= 0.0)
        f_theta_h = 1.0;
    else if (los_params->theta_h1__rad >= 1.0)
        f_theta_h = 0.0;
    else
        f_theta_h = MAX(0.5 - (1 / PI) * (atan(20.0 * log10(32.0 * los_params->theta_h1__rad))), 0);

    double Y_e__db, Y_e_50__db, A_Y;
    LongTermVariability(terminal_1->d_r__km, terminal_2->d_r__km, d__km, f__mhz, p, f_theta_h, los_params->A_LOS__db, &Y_e__db, &A_Y);
    LongTermVariability(terminal_1->d_r__km, terminal_2->d_r__km, d__km, f__mhz, 50, f_theta_h, los_params->A_LOS__db, &Y_e_50__db, &A_Y);

    // [Eqn 13-2]
    double F_AY;
    if (A_Y <= 0.0)
        F_AY = 1.0;
    else if (A_Y >= 9.0)
        F_AY = 0.1;
    else
        F_AY = (1.1 + (0.9 * cos((A_Y / 9.0) * PI))) / 2.0;

    // [Eqn 175]
    double F_delta_r;
    if (los_params->delta_r__km >= (lambda__km / 2.0))
        F_delta_r = 1.0;
    else if (los_params->delta_r__km <= lambda__km / 6.0)
        F_delta_r = 0.1;
    else
        F_delta_r = 0.5 * (1.1 - (0.9 * cos(((3.0 * PI) / lambda__km) * (los_params->delta_r__km - (lambda__km / 6.0)))));

    double R_s = R_Tg * F_delta_r * F_AY;       // [Eqn 13-4]

    double Y_pi_99__db = 10.0 * log10(f__mhz * pow(result_slant.a__km, 3)) - 84.26;	// [Eqn 13-5]
    double K_t = FindKForYpiAt99Percent(Y_pi_99__db);

    double W_a = pow(10.0, K_t / 10.0);         // [Eqn 13-6]
    double W_R = pow(R_s, 2) + pow(0.01, 2);    // [Eqn 13-7]
    double W = W_R + W_a;                       // [Eqn 13-8]

    // [Eqn 13-9]
    if (W <= 0.0)
        *K_LOS = -40.0;
    else
    {
        *K_LOS = 10.0 * log10(W);

    if (*K_LOS < -40.0)
        *K_LOS = -40.0;
    }

    double Y_pi_50__db = 0.0;   //  zero mean
    double Y_pi__db = NakagamiRice(*K_LOS, p);

    double Y_total__db = -CombineDistributions(Y_e_50__db, Y_e__db, Y_pi_50__db, Y_pi__db, p);

    //
    // Compute variability
    /////////////////////////////////////////////

    result->d__km = los_params->d__km;
    result->A__db = result->A_fs__db + result->A_a__db - los_params->A_LOS__db + Y_total__db;
    result->theta_h1__rad = los_params->theta_h1__rad;
}
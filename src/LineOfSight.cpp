#include <math.h>
#include "..\include\p528.h"

double FindPsiAtDistance(double d__km, Path path, Terminal terminal_1, Terminal terminal_2)
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
		RayOptics(path, terminal_1, terminal_2, psi, &params_temp);

		d_psi__km = params_temp.d__km;

		// compute delta
		if (d_psi__km > d__km)
			delta_psi = abs(delta_psi) / 2;
		else
			delta_psi = -abs(delta_psi) / 2;

	} while (abs(d__km - d_psi__km) > 1e-3 && (abs(delta_psi) > 1e-12));  // get within 1 meter of desired delta_r value

	return psi;
}

double FindPsiAtDeltaR(double delta_r, Path path, Terminal terminal_1, Terminal terminal_2)
{
	double psi = PI / 2;
	double delta_psi = -PI / 4;

	LineOfSightParams params_temp;
	do
	{
		psi += delta_psi;

		RayOptics(path, terminal_1, terminal_2, psi, &params_temp);

		if (params_temp.delta_r > delta_r)
			delta_psi = -abs(delta_psi) / 2;
		else
			delta_psi = abs(delta_psi) / 2;

	} while (abs(params_temp.delta_r - delta_r) > 1e-6);  // get within 1 millimeter of desired delta_r value

	return psi;
}

double FindDistanceAtDeltaR(double delta_r, Path path, Terminal terminal_1, Terminal terminal_2)
{
	double psi = PI / 2;
	double delta_psi = -PI / 4;

	LineOfSightParams params_temp;
	do
	{
		psi += delta_psi;

		RayOptics(path, terminal_1, terminal_2, psi, &params_temp);

		if (params_temp.delta_r > delta_r)
			delta_psi = -abs(delta_psi) / 2;
		else
			delta_psi = abs(delta_psi) / 2;

	} while (abs(params_temp.delta_r - delta_r) > 1e-6);  // get within 1 millimeter of desired delta_r value

	return params_temp.d__km;
}

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
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
 |                q             - Time percentage
 |                d__km         - Path length, in km
 |                T_pol         - Code indicating either polarization
 |                                  + 0 : POLARIZATION__HORIZONTAL
 |                                  + 1 : POLARIZATION__VERTICAL
 |
 |      Outputs:  los_params    - Struct contianing LOS parameters
 |                result        - Struct contianing P.528 results
 |                K_LOS         - K-value
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void LineOfSight(Path *path, Terminal terminal_1, Terminal terminal_2, LineOfSightParams *los_params, 
    double f__mhz, double A_dML__db, double q, double d__km, int T_pol, Result *result, double *K_LOS)
{
    double psi;
    double R_Tg;

	// 0.2997925 = speed of light, gigameters per sec
    double lambda__km = 0.2997925 / f__mhz;                             // [Eqn 49]

	// determine psi_limit, where you switch from free space to 2-ray model
	// lambda / 2 is the start of the lobe closest to d_ML
	double psi_limit = FindPsiAtDeltaR(lambda__km / 2, *path, terminal_1, terminal_2);

	// "[d_y6__km] is the largest distance at which a free-space value is obtained in a two-ray model
	//   of reflection from a smooth earth with a reflection coefficient of -1" [ES-83-3, page 44]
	double d_y6__km = FindDistanceAtDeltaR(lambda__km / 6, *path, terminal_1, terminal_2);

    /////////////////////////////////////////////
    // Determine d_0__km distance
    //

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
        psi = FindPsiAtDistance(d_temp__km, *path, terminal_1, terminal_2);

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

	double psi_d0 = FindPsiAtDistance(path->d_0__km, *path, terminal_1, terminal_2);

	RayOptics(*path, terminal_1, terminal_2, psi_d0, los_params);

	GetPathLoss(psi_d0, *path, terminal_1, terminal_2, f__mhz, psi_limit, A_dML__db, 0, T_pol, los_params, &R_Tg);

	//
	// Compute loss at d_0__km
	/////////////////////////////////////////////

	// tune psi for the desired distance
	psi = FindPsiAtDistance(d__km, *path, terminal_1, terminal_2);

	RayOptics(*path, terminal_1, terminal_2, psi, los_params);

	GetPathLoss(psi, *path, terminal_1, terminal_2, f__mhz, psi_limit, A_dML__db, los_params->A_LOS__db, T_pol, los_params, &R_Tg);

	/////////////////////////////////////////////
	// Compute atmospheric absorption
	//

	double r_eo__km = CalculateEffectiveRayLength(los_params->z__km[0], los_params->z__km[1], los_params->a_a__km, los_params->r_0__km, los_params->theta_h1__rad, T_eo__km);
	double r_ew__km = CalculateEffectiveRayLength(los_params->z__km[0], los_params->z__km[1], los_params->a_a__km, los_params->r_0__km, los_params->theta_h1__rad, T_ew__km);

	double gamma_oo, gamma_ow;
	AtmosphericAbsorptionParameters(f__mhz, &gamma_oo, &gamma_ow);

	result->A_a__db = gamma_oo * r_eo__km + gamma_ow * r_ew__km;

	//
	// Compute atmospheric absorption
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// Compute free-space loss
	//

	double theta_fs = ((los_params->theta[0] + los_params->theta[1]) * los_params->a_a__km) / a_0__km;

	double z_1__km = a_0__km + terminal_1.h_r__km;                          // [Eqn 57]
	double z_2__km = a_0__km + terminal_2.h_r__km;                          // [Eqn 57]

	double r_fs__km = MAX(sqrt(pow(z_2__km - z_1__km, 2) + (4.0 * z_1__km * z_2__km * pow(sin(theta_fs * 0.5), 2))), fabs(z_2__km - z_1__km));   // [Eqn 58]

	double L_bf__db = 32.45 + 20.0 * log10(f__mhz);                        // [Eqn 59]
	result->A_fs__db = L_bf__db + 20.0 * log10(r_fs__km);                   // [Eqn 60]

	//
	// Compute free-space loss
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// Compute variability
	//

	double f_theta_h;
	if (los_params->theta_h1__rad <= 0.0)
		f_theta_h = 1.0;
	else if (los_params->theta_h1__rad >= 1.0)
		f_theta_h = 0.0;
	else
		f_theta_h = MAX(0.5 - 0.3183098862 * (atan(20.0 * log10(32.0 * los_params->theta_h1__rad))), 0);

	double Y_e__db, Y_e_50__db, A_Y;
	LongTermVariability(terminal_1.h_r__km, terminal_2.h_r__km, d__km, f__mhz, q, f_theta_h, los_params->A_LOS__db, &Y_e__db, &A_Y);
	LongTermVariability(terminal_1.h_r__km, terminal_2.h_r__km, d__km, f__mhz, 50, f_theta_h, los_params->A_LOS__db, &Y_e_50__db, &A_Y);

	double F_AY;
	if (A_Y <= 0.0)
		F_AY = 1.0;
	else if (A_Y >= 9.0)
		F_AY = 0.1;
	else
		F_AY = (1.1 + (0.9 * cos((A_Y / 9.0) * PI))) / 2.0;

    // [Eqn 175]
	double F_delta_r;
	if (los_params->delta_r >= (lambda__km / 2.0))
		F_delta_r = 1.0;
	else if (los_params->delta_r <= lambda__km / 6.0)
		F_delta_r = 0.1;
	else
		F_delta_r = 0.5 * (1.1 - (0.9 * cos(((3.0 * PI) / lambda__km) * (los_params->delta_r - (lambda__km / 6.0)))));

	double R_s = R_Tg * F_delta_r * F_AY;       // [Eqn 178]

	double W_a;
	if (r_ew__km <= 0.0)
		W_a = 0.0001;
	else
	{
		double Y_pi_99__db = 10.0 * log10(f__mhz * pow(r_ew__km, 3)) - 84.26;
		double K_t = FindKForYpiAt99Percent(Y_pi_99__db);

		W_a = pow(10.0, K_t / 10.0);
	}

	double W_R = pow(R_s, 2) + pow(0.01, 2);        // [Eqn 180]

	double W = W_R + W_a;                           // [Eqn 181]

    // [Eqn 183]
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

	double Y_total__db = -CombineDistributions(Y_e_50__db, Y_e__db, Y_pi_50__db, Y_pi__db, q);

	//
	// Compute variability
	/////////////////////////////////////////////

	result->d__km = los_params->d__km;
	result->A__db = result->A_fs__db + result->A_a__db - los_params->A_LOS__db + Y_total__db;
}
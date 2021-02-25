#include <math.h>
#include "..\include\p528.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This file computes the atmospheric loss for a NLOS path
 |                as described in Annex 2, Section 13 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  terminal_1    -
 |                terminal_2    -
 |                path          -
 |                tropo         -
 |                f__mhz        - Frequency, in MHz
 |
 |      Outputs:  A_a__db       - Atmospheric absorption loss, in dB
 |
 *===========================================================================*/
double TranshorizonAtmosphericAbsorptionLoss(Terminal terminal_1, Terminal terminal_2, Path path, TroposcatterParams tropo, double f__mhz)
{
	double z_1__km = terminal_1.h__km + path.a_e__km;
	double z_2__km = terminal_2.h__km + path.a_e__km;
	double z_v__km = tropo.h_v__km + path.a_e__km;

	double z_low1__km, z_high1__km, beta_1__rad;
	if (z_1__km > z_v__km)
	{
		z_low1__km = z_v__km;
		z_high1__km = z_1__km;
		beta_1__rad = -atan(tropo.theta_A);
	}
	else
	{
		z_low1__km = z_1__km;
		z_high1__km = z_v__km;
		beta_1__rad = -terminal_1.theta__rad;
	}

	double z_low2__km, z_high2__km, beta_2__rad;
	if (z_2__km > z_v__km)
	{
		z_low2__km = z_v__km;
		z_high2__km = z_2__km;
		beta_2__rad = -atan(tropo.theta_A);
	}
	else
	{
		z_low2__km = z_2__km;
		z_high2__km = z_v__km;
		beta_2__rad = -terminal_2.theta__rad;
	}

	// Step 4
	double d_arc1__km = terminal_1.d__km + tropo.d_z__km;
	double d_arc2__km = terminal_2.d__km + tropo.d_z__km;

	// Step 5
	double r_eo1__km = CalculateEffectiveRayLength(z_low1__km, z_high1__km, path.a_e__km, d_arc1__km, beta_1__rad, T_eo__km);
	double r_ew1__km = CalculateEffectiveRayLength(z_low1__km, z_high1__km, path.a_e__km, d_arc1__km, beta_1__rad, T_ew__km);

	// Step 6
	double r_eo2__km = CalculateEffectiveRayLength(z_low2__km, z_high2__km, path.a_e__km, d_arc2__km, beta_2__rad, T_eo__km);
	double r_ew2__km = CalculateEffectiveRayLength(z_low2__km, z_high2__km, path.a_e__km, d_arc2__km, beta_2__rad, T_ew__km);

	// Step 7
	double r_eo__km = r_eo1__km + r_eo2__km;
	double r_ew__km = r_ew1__km + r_ew2__km;

	// Step 8
	double gamma_oo, gamma_ow;
	AtmosphericAbsorptionParameters(f__mhz, &gamma_oo, &gamma_ow);

	// Step 9
	double A_a__db = gamma_oo * r_eo__km + gamma_ow * r_ew__km;

	return A_a__db;
}
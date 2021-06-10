#include <math.h>
#include "..\..\include\p528.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This file computes the Troposcatter loss
 |                as described in Annex 2, Section 11 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  path          - Struct containing path parameters
 |                terminal_1    - Struct containing low terminal parameters
 |                terminal_2    - Struct containing high terminal parameters
 |                d__km         - Path distance, in km
 |                f__mhz        - Frequency, in MHz
 |
 |      Outputs:  tropo         - Struct containing resulting parameters
 |
 *===========================================================================*/
void Troposcatter(Path *path, Terminal *terminal_1, Terminal *terminal_2, double d__km, double f__mhz, TroposcatterParams *tropo)
{
	double Q_o, Q_a, Q_b, Q_A, Q_B;
	double z_a__km, z_b__km, Z_a__km, Z_b__km;

	tropo->d_s__km = d__km - terminal_1->d_r__km - terminal_2->d_r__km;		// [Eqn 11-2]

	if (tropo->d_s__km <= 0.0)
	{
		tropo->d_z__km = 0.0;
		tropo->A_s__db = 0.0;
		tropo->d_s__km = 0.0;
		tropo->h_v__km = 0.0;
		tropo->theta_s = 0.0;
		tropo->theta_A = 0.0;
	}
	else
	{
		///////////////////////////////////////
		// Compute the geometric parameters
		//

		tropo->d_z__km = 0.5 * tropo->d_s__km;                                // [Eqn 11-6]

		double A_m = 1 / a_0__km;                                             // [Eqn 11-7]
		double dN = A_m - (1.0 / a_e__km);									  // [Eqn 11-8]
		double gamma_e__km = (N_s * 1e-6) / dN;                               // [Eqn 11-9]

		z_a__km = 1.0 / (2 * a_e__km) * pow(tropo->d_z__km / 2, 2);            // [Eqn 11-10]
		z_b__km = 1.0 / (2 * a_e__km) * pow(tropo->d_z__km, 2);                // [Eqn 11-11]

		Q_o = A_m - dN;                                                             // [Eqn 11-12]

		Q_a = A_m - dN / exp(MIN(35.0, z_a__km / gamma_e__km));                     // [Eqn 11-13]
		Q_b = A_m - dN / exp(MIN(35.0, z_b__km / gamma_e__km));                     // [Eqn 11-13]

		Z_a__km = (7.0 * Q_o + 6.0 * Q_a - Q_b) * (pow(tropo->d_z__km, 2) / 96.0);  // [Eqn 11-14]
		Z_b__km = (Q_o + 2.0 * Q_a) * (pow(tropo->d_z__km, 2) / 6.0);               // [Eqn 11-15]

		Q_A = A_m - dN / exp(MIN(35.0, Z_a__km / gamma_e__km));                     // [Eqn 11-16]
		Q_B = A_m - dN / exp(MIN(35.0, Z_b__km / gamma_e__km));                     // [Eqn 11-16]

		tropo->h_v__km = (Q_o + 2.0 * Q_A) * (pow(tropo->d_z__km, 2) / 6.0);        // [Eqn 11-17]

		tropo->theta_A = (Q_o + 4.0 * Q_A + Q_B) * tropo->d_z__km / 6.0;            // [Eqn 11-18]

		tropo->theta_s = 2 * tropo->theta_A;                                        // [Eqn 11-19]

		//
		// Compute the geometric parameters
		///////////////////////////////////////

		///////////////////////////////////////
		// Compute the scattering efficiency term
		// 
		double epsilon_1 = 5.67e-6 * pow(N_s, 2) - 0.00232 * N_s + 0.031;           // [Eqn 11-20]
		double epsilon_2 = 0.0002 * pow(N_s, 2) - 0.06 * N_s + 6.6;                 // [Eqn 11-21]

		double gamma = 0.1424 * (1.0 + epsilon_1 / exp(MIN(35.0, pow(tropo->h_v__km / 4.0, 6))));   // [Eqn 11-22]

		double S_e__db = 83.1 - epsilon_2 / (1.0 + 0.07716 * pow(tropo->h_v__km, 2)) + 20 * log10(pow(0.1424 / gamma, 2) * exp(gamma * tropo->h_v__km));    // [Eqn 11-23]

		//
		// Compute the scattering efficiency term
		///////////////////////////////////////

		///////////////////////////////////////
		// Compute the scattering volume term
		// 

		double X_A1__km2 = pow(terminal_1->h_e__km, 2) + 4.0 * (a_e__km + terminal_1->h_e__km) * a_e__km * pow(sin(terminal_1->d_r__km / (a_e__km * 2)), 2);      // [Eqn 11-24]
		double X_A2__km2 = pow(terminal_2->h_e__km, 2) + 4.0 * (a_e__km + terminal_2->h_e__km) * a_e__km * pow(sin(terminal_2->d_r__km / (a_e__km * 2)), 2);      // [Eqn 11-24]

		double ell_1__km = sqrt(X_A1__km2) + tropo->d_z__km;                        // [Eqn 11-25]
		double ell_2__km = sqrt(X_A2__km2) + tropo->d_z__km;                        // [Eqn 11-25]
		double ell__km = ell_1__km + ell_2__km;                                     // [Eqn 11-26]

		double s = (ell_1__km - ell_2__km) / ell__km;                               // [Eqn 11-27]
		double eta = gamma * tropo->theta_s * ell__km / 2;                          // [Eqn 11-28]

		double kappa = f__mhz / 0.0477;                                             // [Eqn 11-29]

		double rho_1__km = 2.0 * kappa * tropo->theta_s * terminal_1->h_e__km;         // [Eqn 11-30]
		double rho_2__km = 2.0 * kappa * tropo->theta_s * terminal_2->h_e__km;         // [Eqn 11-30]

		double SQRT2 = sqrt(2);

		double A = pow(1 - pow(s, 2), 2);                                           // [Eqn 11-36]

		double X_v1 = pow(1 + s, 2) * eta;                                          // [Eqn 11-32]
		double X_v2 = pow(1 - s, 2) * eta;                                          // [Eqn 11-33]

		double q_1 = pow(X_v1, 2) + pow(rho_1__km, 2);                              // [Eqn 11-34]
		double q_2 = pow(X_v2, 2) + pow(rho_2__km, 2);                              // [Eqn 11-35]

        // [Eqn 11-37]
		double B_s = 6 + 8 * pow(s, 2)
			+ 8 * (1.0 - s) * pow(X_v1, 2) * pow(rho_1__km, 2) / pow(q_1, 2)
			+ 8 * (1.0 + s) * pow(X_v2, 2) * pow(rho_2__km, 2) / pow(q_2, 2)
			+ 2 * (1.0 - pow(s, 2)) * (1 + 2 * pow(X_v1, 2) / q_1) * (1 + 2 * pow(X_v2, 2) / q_2);

        // [Eqn 11-38]
		double C_s = 12
			* pow((rho_1__km + SQRT2) / rho_1__km, 2)
			* pow((rho_2__km + SQRT2) / rho_2__km, 2)
			* (rho_1__km + rho_2__km) / (rho_1__km + rho_2__km + 2 * SQRT2);

		double temp = (A * pow(eta, 2) + B_s * eta) * q_1 * q_2 / (pow(rho_1__km, 2) * pow(rho_2__km, 2));

		double S_v__db = 10 * log10(temp + C_s);

		//
		// Compute the scattering volume term
		///////////////////////////////////////

		tropo->A_s__db = S_e__db + S_v__db + 10.0 * log10(kappa * pow(tropo->theta_s, 3) / ell__km);
	}
}
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
 |  Description:  This function computes the reflection coefficients
 |                as described in Annex 2, Section 9 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  psi       - Reflection angle
 |                f__mhz    - Frequency
 |                T_pol     - Code indicating either polarization
 |                              + 0 : POLARIZATION__HORIZONTAL
 |                              + 1 : POLARIZATION__VERTICAL
 |
 |      Outputs:  R_g       - Real part
 |                phi_g     - Imaginary part
 |
 *===========================================================================*/
void ReflectionCoefficients(double psi, double f__mhz, int T_pol, double *R_g, double *phi_g)
{
	double sin_psi, cos_psi;
	if (psi <= 0.0)
	{
		psi = 0.0;
		sin_psi = 0.0;
		cos_psi = 1.0;
	}
	else if (psi >= PI / 2)
	{
		psi = PI / 2;
		sin_psi = 1.0;
		cos_psi = 0.0;
	}
	else
	{
		sin_psi = sin(psi);
		cos_psi = cos(psi);
	}

	double X = (18000.0 * sigma) / f__mhz;				// [Eqn 89]
	double Y = epsilon - pow(cos_psi, 2);				// [Eqn 90]

	double T = sqrt(pow(Y, 2) + pow(X, 2)) + Y;			// [Eqn 91]

	double P = sqrt(T * 0.5);							// [Eqn 92]

	double Q = X / (2.0 * P);							// [Eqn 93]

	// [Eqn 94]
	double B;
	if (T_pol == POLARIZATION__HORIZONTAL)
		B = 1.0 / (pow(P, 2) + pow(Q, 2));
	else
		B = (pow(epsilon, 2) + pow(X, 2)) / (pow(P, 2) + pow(Q, 2));

	// [Eqn 95]
	double A;
	if (T_pol == POLARIZATION__HORIZONTAL)
		A = (2.0 * P) / (pow(P, 2) + pow(Q, 2));
	else
		A = (2.0 * (P * epsilon + Q * X)) / (pow(P, 2) + pow(Q, 2));

	// [Eqn 96]
	*R_g = sqrt((1.0 + (B * pow(sin_psi, 2)) - (A * sin_psi)) / (1.0 + (B * pow(sin_psi, 2)) + (A * sin_psi)));

	// [Eqn 97]
	double alpha;
	if (T_pol == POLARIZATION__HORIZONTAL)
		alpha = atan2(-Q, sin_psi - P);
	else
		alpha = atan2((epsilon * sin_psi) - Q, epsilon * sin_psi - P);

	// [Eqn 98]
	double beta;
	if (T_pol == POLARIZATION__HORIZONTAL)
		beta = atan2(Q, sin_psi + P);
	else
		beta = atan2((X * sin_psi) + Q, epsilon * sin_psi + P);

	// [Eqn 99]
	*phi_g = alpha - beta;

	return;
}
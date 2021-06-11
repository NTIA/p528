#include <math.h>
#include "../include/p528.h"

/*=============================================================================
 |
 |  Description:  This function computes the reflection coefficients
 |                as described in Annex 2, Section 9 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  psi       - Reflection angle
 |                f__mhz    - Frequency
 |
 |      Outputs:  R_g       - Real part
 |                phi_g     - Imaginary part
 |
 *===========================================================================*/
void ReflectionCoefficients(double psi, double f__mhz, double *R_g, double *phi_g)
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

    double X = (18000.0 * sigma) / f__mhz;
    double Y = EPSILON - pow(cos_psi, 2);

    double T = sqrt(pow(Y, 2) + pow(X, 2)) + Y;

    double P = sqrt(T * 0.5);

    double Q = X / (2.0 * P);
    double B = 1.0 / (pow(P, 2) + pow(Q, 2));
    double A = (2.0 * P) / (pow(P, 2) + pow(Q, 2));

    *R_g = sqrt((1.0 + (B * pow(sin_psi, 2)) - (A * sin_psi)) / (1.0 + (B * pow(sin_psi, 2)) + (A * sin_psi)));

    *phi_g = atan2(-Q, sin_psi - P) - atan2(Q, sin_psi + P);

    return;
}
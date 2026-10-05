/** @file ReflectionCoefficients.cpp
 * Computes the reflection coefficients
 */
#include <math.h>
#include "p528.h"

/**
 * Computes the reflection coefficients
 *
 * References:
 *     - Recommendation ITU‑R P.528‑5 "Propagation curves for aeronautical mobile
 * and radionavigation services using the VHF, UHF and SHF bands", 
 * Annex 2, Section 8.
 *
 * @param[in]   psi__rad  Reflection angle, in rad
 * @param[in]   f__mhz    Frequency, in MHz
 * @param[in]   T_pol     Code indicating either polarization
 *                          + 0 : POLARIZATION__HORIZONTAL
 *                          + 1 : POLARIZATION__VERTICAL
 * @param[out]  R_g       Real part
 * @param[out]  phi_g     Imaginary part
 * @par Returns
 *      Nothing.
 ******************************************************************************/
void ReflectionCoefficients(double psi__rad, double f__mhz, int T_pol, double *R_g, double *phi_g)
{
    double sin_psi, cos_psi;
    if (psi__rad <= 0.0)
    {
        psi__rad = 0.0;
        sin_psi = 0.0;
        cos_psi = 1.0;
    }
    else if (psi__rad >= PI / 2)
    {
        psi__rad = PI / 2;
        sin_psi = 1.0;
        cos_psi = 0.0;
    }
    else
    {
        sin_psi = sin(psi__rad);
        cos_psi = cos(psi__rad);
    }

    double X = (18000.0 * sigma) / f__mhz;              // [Eqn 9-1]
    double Y = epsilon_r - pow(cos_psi, 2);               // [Eqn 9-2]
    double T = sqrt(pow(Y, 2) + pow(X, 2)) + Y;         // [Eqn 9-3]
    double P = sqrt(T * 0.5);                           // [Eqn 9-4]
    double Q = X / (2.0 * P);                           // [Eqn 9-5]

    // [Eqn 9-6]
    double B;
    if (T_pol == POLARIZATION__HORIZONTAL)
        B = 1.0 / (pow(P, 2) + pow(Q, 2));
    else
        B = (pow(epsilon_r, 2) + pow(X, 2)) / (pow(P, 2) + pow(Q, 2));

    // [Eqn 9-7]
    double A;
    if (T_pol == POLARIZATION__HORIZONTAL)
        A = (2.0 * P) / (pow(P, 2) + pow(Q, 2));
    else
        A = (2.0 * (P * epsilon_r + Q * X)) / (pow(P, 2) + pow(Q, 2));

    // [Eqn 9-8]
    *R_g = sqrt((1.0 + (B * pow(sin_psi, 2)) - (A * sin_psi)) / (1.0 + (B * pow(sin_psi, 2)) + (A * sin_psi)));

    // [Eqn 9-9]
    double alpha;
    if (T_pol == POLARIZATION__HORIZONTAL)
        alpha = atan2(-Q, sin_psi - P);
    else
        alpha = atan2((epsilon_r * sin_psi) - Q, epsilon_r * sin_psi - P);

    // [Eqn 9-10]
    double beta;
    if (T_pol == POLARIZATION__HORIZONTAL)
        beta = atan2(Q, sin_psi + P);
    else
        beta = atan2((X * sin_psi) + Q, epsilon_r * sin_psi + P);

    // [Eqn 9-11]
    *phi_g = alpha - beta;
}

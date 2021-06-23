#include <math.h>
#include <complex>
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
 |  Description:  This function computes the line of sight loss
 |                as described in Annex 2, Section 8 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  psi__rad      - Reflection angle, in rad
 |                path          - Struct containing path parameters
 |                f__mhz        - Frequency, in MHz
 |                psi_limit     - Angular limit separating FS and 2-Ray, in rad
 |                A_dML__db     - Diffraction loss at d_ML, in dB
 |                A_d_0__db     - Loss at d_0, in dB
 |                T_pol         - Code indicating either polarization
 |                                  + 0 : POLARIZATION__HORIZONTAL
 |                                  + 1 : POLARIZATION__VERTICAL
 |
 |      Outputs:  params        - Line of sight loss params
 |                R_Tg          - Reflection parameter
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void GetPathLoss(double psi__rad, Path *path, double f__mhz, double psi_limit, 
    double A_dML__db, double A_d_0__db, int T_pol, 
    LineOfSightParams* params, double *R_Tg)
{
    double R_g, phi_g;
    ReflectionCoefficients(psi__rad, f__mhz, T_pol, &R_g, &phi_g);

    double D_v;
    if (tan(psi__rad) >= 0.1)
        D_v = 1.0;
    else
    {
        double r_1 = params->D__km[0] / cos(psi__rad);       // [Eqn 8-3]
        double r_2 = params->D__km[1] / cos(psi__rad);       // [Eqn 8-3]
        double R_r = (r_1 * r_2) / params->r_12__km;    // [Eqn 8-4]

        double term_1 = (2 * R_r * (1 + pow(sin(psi__rad), 2))) / (params->a_a__km * sin(psi__rad));
        double term_2 = pow(2 * R_r / params->a_a__km, 2);
        D_v = pow(1.0 + term_1 + term_2, -0.5);         // [Eqn 8-5]
    }

    // Ray-length factor, [Eqn 8-6]
    double F_r = MIN(params->r_0__km / params->r_12__km, 1);

    *R_Tg = R_g * D_v * F_r;                            // [Eqn 8-7]

    if (params->d__km > path->d_0__km)
    {
        // [Eqn 8-1]
        params->A_LOS__db = ((params->d__km - path->d_0__km) * (A_dML__db - A_d_0__db) / (path->d_ML__km - path->d_0__km)) + A_d_0__db;
    }
    else
    {
        double lambda__km = 0.2997925 / f__mhz;	// [Eqn 8-2]

        if (psi__rad > psi_limit)
        {
            // ignore the phase lag; Step 8-2
            params->A_LOS__db = 0;
        }
        else
        {
            // Total phase lag of the ground reflected ray relative to the direct ray

            // [Eqn 8-8]
            double phi_Tg = (2 * PI * params->delta_r / lambda__km) + phi_g;

            // [Eqn 8-9]
            std::complex<double> cplx = std::complex<double>(*R_Tg * cos(phi_Tg), -*R_Tg * sin(phi_Tg));

            // [Eqn 8-10]
            double W_RL = MIN(abs(1.0 + cplx), 1.0);

            // [Eqn 8-11]
            double W_R0 = pow(W_RL, 2);

            // [Eqn 8-12]
            params->A_LOS__db = 10.0 * log10(W_R0);
        }
    }
}
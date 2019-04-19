#include <math.h>
#include <complex>
#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  This function computes the line of sight loss
 |                as described in Annex 2, Section 8 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  psi	        - Reflection angle
 |				  path	        - Struct containing path parameters
 |			      terminal_1    - Struct for low terminal parameters
 |                terminal_2    - Struct for high terminal parameters
 |                f__mhz        - Frequency, in MHz
 |                psi_limit     - Angular limit seperating FS and 2-Ray
 |                A_dML__db     - Diffraction loss at d_ML, in dB
 |                A_d_0__db     - Loss at d_0, in dB
 |
 |      Outputs:  params        - Line of sight loss params
 |                R_Tg          - Reflection parameter
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void GetPathLoss(double psi, Path path, Terminal terminal_1, Terminal terminal_2,
    double f__mhz, double psi_limit, double A_dML__db, double A_d_0__db, LineOfSightParams* params, double *R_Tg)
{
    double R_g, phi_g;
    ReflectionCoefficients(psi, f__mhz, &R_g, &phi_g);

    double D_v;             // Divergence factor, [Eqn 78]
    if (tan(psi) >= 0.1)
        D_v = 1.0;
    else
    {
        double r_1 = params->D__km[0] / cos(psi);
        double r_2 = params->D__km[1] / cos(psi);
        double R_r = (r_1 * r_2) / params->r_12__km;

        double term_1 = (2 * R_r * (1 + pow(sin(psi), 2))) / (params->a_a__km * sin(psi));
        double term_2 = pow(2 * R_r / params->a_a__km, 2);
        D_v = pow(1.0 + term_1 + term_2, -0.5);
    }

    double TD3 = params->r_0__km / params->r_12__km;
    if (TD3 > 1.0)
        TD3 = 1.0;

    *R_Tg = R_g * D_v * TD3;

    // [Eqn 177]
    if (params->d__km > path.d_0__km)
    {
        params->A_LOS__db = ((params->d__km - path.d_0__km) * (A_dML__db - A_d_0__db) / (path.d_ML__km - path.d_0__km)) + A_d_0__db;
    }
    else
    {
        double lambda = 0.2997925 / f__mhz;

        double F_r = MIN(params->r_0__km / params->r_12__km, 1);    // Ray-length factor, [Eqn 79]

        double R_Tg = R_g * D_v * F_r;                              // [Eqn 80]

        double WRL;
        if (psi > psi_limit)
            WRL = 1.0;          // ignore the phase lag
        else
        {
            // Total phase lag of the ground reflected ray relative to the direct ray
            double phi_Tg = (2 * PI * params->delta_r / lambda) + phi_g;     // [Eqn 81]

            std::complex<double> cplx = std::complex<double>(R_Tg * cos(phi_Tg), -R_Tg * sin(phi_Tg));
            WRL = MIN(abs(1.0 + cplx), 1.0);
        }

        double W_R0 = pow(WRL, 2) + 0.0001;

        // A_LOS__db = A_R0__db up to d_0__km, [Eqn 85]
        params->A_LOS__db = 10.0 * log10(W_R0);
    }
}
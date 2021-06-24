#include <math.h>
#include "../../include/p528.h"

// References:
//  - [Vogler 1964]   Calculation of Groundwave Attenuation in the Far Diffraction Region.  L. Vogler.  1964
//  - [FAA-ES-83-3]   The IF-77 Electromagnetic Wave Propagation Model.  Gierhart and Johnson.  1983

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This file computes the smooth earth diffraction loss
 |                as described in Annex 2, Section 10 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  d_1__km   - Horizon distance of terminal 1, in km
 |                d_2__km   - Horizon distance of terminal 2, in km
 |                a_e__km   - Effective earth radius, in km
 |                f__mhz    - Frequency, in MHz
 |                d_0__km   - Path length of interest, in km
 |                T_pol     - Code indicating either polarization
 |                              + 0 : POLARIZATION__HORIZONTAL
 |                              + 1 : POLARIZATION__VERTICAL
 |
 |      Returns:  A_d__db   - Diffraction loss, in dB
 |
 *===========================================================================*/


double DistanceFunction(double x__km)
{
    // [Vogler 1964, Equ 13]
    double G_x__db = 0.05751 * x__km - 10.0 * log10(x__km);

    return G_x__db;
}

double HeightFunction(double x__km, double K)
{
    double F_x__db;

    // [FAA-ES-83-3, Equ 73]
    double y__db = 40.0 * log10(x__km) - 117.0;

    // [Vogler 1964, Equ 13]
    double G_x__db = DistanceFunction(x__km);

    if (x__km <= 200.0)
    {
        double x_t__km = 450 / -pow(log10(K), 3);       // [Eqn 109]

        // [Eqn 110]
        if (x__km >= x_t__km)
        {
            if (fabs(y__db) < 117)
                F_x__db = y__db;
            else
                F_x__db = -117;
        }
        else
            F_x__db = 20 * log10(K) - 15 + (0.000025 * pow(x__km, 2) / K);
    }
    else if (x__km > 2000.0)
    {
        // [Vogler 1964] F_x ~= G_x for large x (see Figure 7)
        F_x__db = G_x__db;
    }
    else // Blend y__db with G_x__db for 200 < x__km < 2000
    {
        // [FAA-ES-83-3, Equ 72] weighting variable
        double W = 0.0134 * x__km * exp(-0.005 * x__km);

        // [FAA-ES-83-3, Equ 75]
        F_x__db = W * y__db + (1.0 - W) * G_x__db;
    }

    return F_x__db;
}

double SmoothEarthDiffraction(double d_1__km, double d_2__km, double f__mhz, double d_0__km, int T_pol)
{
    double s = 18000 * sigma / f__mhz;

    double K;
    if (T_pol == POLARIZATION__HORIZONTAL)
        K = 0.01778 * pow(f__mhz, -THIRD) * pow(pow(epsilon_r - 1, 2) + pow(s, 2), -0.25);
    else
       K = 0.01778 * pow(f__mhz, -THIRD) * pow((pow(epsilon_r, 2) + pow(s, 2)) / pow(pow(epsilon_r - 1, 2) + pow(s, 2), 0.5), 0.5);

    double B_0 = 1.607;

    // [Vogler 1964, Equ 2] with C_0 = 1 due to "4/3" Earth assumption
    double x_0__km = (B_0 - K) * pow(f__mhz, THIRD) * d_0__km;
    double x_1__km = (B_0 - K) * pow(f__mhz, THIRD) * d_1__km;
    double x_2__km = (B_0 - K) * pow(f__mhz, THIRD) * d_2__km;

    // Compute the distance function for the path
    double G_x__db = DistanceFunction(x_0__km);

    // Compute the height functions for the two terminals
    double F_x1__db = HeightFunction(x_1__km, K);
    double F_x2__db = HeightFunction(x_2__km, K);

    // [Vogler 1964, Equ 1] with C_1(K, b^0) = 20, which is the approximate value for all K (see Figure 5)
    return G_x__db - F_x1__db - F_x2__db - 20.0;
}
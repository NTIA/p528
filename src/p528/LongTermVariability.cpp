#include <math.h>
#include "../../include/p528.h"

/*=============================================================================
 |
 |  Description:  This function computes the long term variability
 |                as described in Annex 2, Section 14 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  d_r1__km          - Actual height of low terminal, in km
 |                d_r2__km          - Actual height of high terminal, in km
 |                d__km             - Path distance, in km
 |                f__mhz            - Frequency, in MHz
 |                p                 - Time percentage
 |
 |      Outputs:  Y_e__db           - Variability, in dB
 |                A_Y               - Conditional adjustment factor, in dB
 |
 *===========================================================================*/
void LongTermVariability(double d_r1__km, double d_r2__km, double d__km, double f__mhz,
    double p, double f_theta_h, double A_T, double *Y_e__db, double *A_Y) 
{
    double d_qs__km = 65.0 * pow((100.0 / f__mhz), THIRD);              // [Eqn 14-1]
    double d_Lq__km = d_r1__km + d_r2__km;                              // [Eqn 14-2]
    double d_q__km = d_Lq__km + d_qs__km;                               // [Eqn 14-3]

    // [Eqn 14-4]
    double d_e__km;
    if (d__km <= d_q__km)
        d_e__km = (130.0 * d__km) / d_q__km;
    else
        d_e__km = 130.0 + d__km - d_q__km;

    // [Eqns 14-5 and 14-6]
    double g_10, g_90;
    if (f__mhz > 1600.0)
    {
        g_10 = 1.05;
        g_90 = 1.05;
    }
    else
    {
        g_10 = (0.21 * sin(5.22 * log10(f__mhz / 200.0))) + 1.28;
        g_90 = (0.18 * sin(5.22 * log10(f__mhz / 200.0))) + 1.23;
    }

    // Data Source for Below Consts: Tech Note 101, Vol 2
    // Column 1: Table III.4, Row A* (Page III-50)
    // Column 2: Table III.3, Row A* (Page III-49)
    // Column 3: Table III.5, Row Continental Temperate (Page III-51)

    double c_1[] = { 2.93e-4, 5.25e-4, 1.59e-5 };
    double c_2[] = { 3.78e-8, 1.57e-6, 1.56e-11 };
    double c_3[] = { 1.02e-7, 4.70e-7, 2.77e-8 };

    double n_1[] = { 2.00, 1.97, 2.32 };
    double n_2[] = { 2.88, 2.31, 4.08 };
    double n_3[] = { 3.15, 2.90, 3.25 };

    double f_inf[] = { 3.2, 5.4, 0.0 };
    double f_m[] = { 8.2, 10.0, 3.9 };

    double Z__db[3];    // = [Y_0(90) Y_0(10) V(50)]
    for (int i = 0; i < 3; i++)
    {
        double f_2 = f_inf[i] + ((f_m[i] - f_inf[i]) * exp(-c_2[i] * pow(d_e__km, n_2[i])));

        Z__db[i] = (c_1[i] * pow(d_e__km, n_1[i]) - f_2) * exp(-c_3[i] * pow(d_e__km, n_3[i])) + f_2;
    }

    double Y_p__db;
    if (p == 50)
        Y_p__db = Z__db[2];
    else if (p > 50)
    {
        double z_90 = InverseComplementaryCumulativeDistributionFunction(90.0 / 100.0);
        double z_p = InverseComplementaryCumulativeDistributionFunction(p / 100.0);
        double c_p = z_p / z_90;

        double Y = c_p * (-Z__db[0] * g_90);
        Y_p__db = Y + Z__db[2];
    }
    else
    {
        double c_p;
        if (p >= 10)
        {
            double z_10 = InverseComplementaryCumulativeDistributionFunction(10.0 / 100.0);
            double z_p = InverseComplementaryCumulativeDistributionFunction(p / 100.0);
            c_p = z_p / z_10;
        }
        else
        {
            // Source for values p < 10: [15], Table 10, Page 34, Climate 6
            vector<double> ps = { 1, 2, 5, 10 };
            vector<double> c_ps = { 1.9507, 1.7166, 1.3265, 1.0000 };

            auto upper = upper_bound(data::P.begin(), data::P.end(), p);
            auto dist = distance(data::P.begin(), upper);
            c_p = LinearInterpolation(ps[dist - 1], c_ps[dist - 1], ps[dist], c_ps[dist], p);
        }

        double Y = c_p * (Z__db[1] * g_10);
        Y_p__db = Y + Z__db[2];
    }

    double Y_10__db = (Z__db[1] * g_10) + Z__db[2]; // [Eqn 14-20]
    double Y_eI__db = f_theta_h * Y_p__db;          // [Eqn 14-21]
    double Y_eI_10__db = f_theta_h * Y_10__db;      // [Eqn 14-22]

    // A_Y "is used to prevent available signal powers from exceeding levels expected for free-space propagation by an unrealistic
    //      amount when the variability about L_b(50) is large and L_b(50) is near its free-space level" [ES-83-3, p3-4]

    double A_YI = (A_T + Y_eI_10__db) - 3.0;        // [Eqn 14-23]
    *A_Y = MAX(A_YI, 0);                            // [Eqn 14-24]
    *Y_e__db = Y_eI__db - *A_Y;                     // [Eqn 14-25]

    // For percentages less than 10%, do a correction check to, 
    //    "prevent available signal powers from exceeding levels expected from free-space levels
    //     by unrealistic amounts" [Gierhart 1970]
    if (p < 10)
    {
        vector<double> c_Y = { -5.0, -4.5, -3.7, 0.0 };

        auto upper = upper_bound(data::P.begin(), data::P.end(), p);
        auto dist = distance(data::P.begin(), upper);
        double c_Yi = LinearInterpolation(data::P[dist - 1], c_Y[dist - 1], data::P[dist], c_Y[dist], p);

        *Y_e__db += A_T;

        if (*Y_e__db > -c_Yi)
            *Y_e__db = -c_Yi;

        *Y_e__db -= A_T;
    }
}
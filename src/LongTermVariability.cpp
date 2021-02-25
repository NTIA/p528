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
 |  Description:  This function computes the long term variability
 |                as described in Annex 2, Section 17 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  h_r1__km          - Actual height of low terminal
 |                h_r2__km          - Actual height of high terminal
 |                d__km             - Path distance, in km
 |                f__mhz            - Frequency, in MHz
 |                time_percentage   - Time percentage
 |
 |      Outputs:  Y_e__db           - Variability, in dB
 |                A_Y               - Value used later
 |
 *===========================================================================*/
void LongTermVariability(double h_r1__km, double h_r2__km, double d__km, double f__mhz,
	double time_percentage, double f_theta_h, double A_T, double *Y_e__db, double *A_Y) 
{
	// Step 1
	double d_Lq1__km, d_Lq2__km, theta__rad;
	RayTrace(N_9000, h_r1__km, &d_Lq1__km, &theta__rad);
	RayTrace(N_9000, h_r2__km, &d_Lq2__km, &theta__rad);

	// Step 2
	double d_qs__km = 65.0 * pow((100.0 / f__mhz), 1.0 / 3.0);          // [Eqn 187]
	double d_Lq__km = d_Lq1__km + d_Lq2__km;                            // [Eqn 188]
	double d_q__km = d_Lq__km + d_qs__km;								// [Eqn 189]

	// [Eqn 190]
	double d_e__km;
	if (d__km <= d_q__km)
		d_e__km = (130.0 * d__km) / d_q__km;
	else
		d_e__km = 130.0 + d__km - d_q__km;

	// Step 3
	double g_1, g_9;
	if (f__mhz > 1600.0)
	{
		g_1 = 1.05;
		g_9 = 1.05;
	}
	else
	{
		g_1 = (0.21 * sin(5.22 * log10(f__mhz / 200.0))) + 1.28;
		g_9 = (0.18 * sin(5.22 * log10(f__mhz / 200.0))) + 1.23;
	}

	// Step 4
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

	double Z__db[3];    // = [Y_0(0.9) Y_0(0.1) V(0.5)]
	for (int i = 0; i < 3; i++)
	{
		double f_2 = f_inf[i] + ((f_m[i] - f_inf[i]) * exp(-c_2[i] * pow(d_e__km, n_2[i])));

		Z__db[i] = (c_1[i] * pow(d_e__km, n_1[i]) - f_2) * exp(-c_3[i] * pow(d_e__km, n_3[i])) + f_2;
	}

	double Y_q__db;
	if (time_percentage == 0.5)
		Y_q__db = Z__db[2];
	else if (time_percentage > 0.5)
	{
		double z_9 = InverseComplementaryCumulativeDistributionFunction(0.9);
		double z_q = InverseComplementaryCumulativeDistributionFunction(time_percentage);
		double c_q = z_q / z_9;

		double Y = c_q * (-Z__db[0] * g_9);
		Y_q__db = Y + Z__db[2];
	}
	else
	{
		double c_q;
		if (time_percentage >= 0.1)
		{
			double z_1 = InverseComplementaryCumulativeDistributionFunction(0.1);
			double z_q = InverseComplementaryCumulativeDistributionFunction(time_percentage);
			c_q = z_q / z_1;
		}
		else
		{
			// Source for values q < 0.10: [15], Table 10, Page 34, Climate 6
			double q[4] = { 0.01, 0.02, 0.05, 0.10 };
			double c[4] = { 1.9507, 1.7166, 1.3265, 1.0000 };

			// find first index greater than 'time_percentage'.  Equality ensures that this will always be true at some point since 0.01 <= time_percentage < 0.1
			for (int i = 1; i < 4; i++)
			{
				if (time_percentage <= q[i])
				{
					c_q = LinearInterpolation(q[i - 1], c[i - 1], q[i], c[i], time_percentage);
					break;
				}
			}
		}

		double Y = c_q * (Z__db[1] * g_1);
		Y_q__db = Y + Z__db[2];
	}

	// Step 6
	double Y_10__db = (Z__db[1] * g_1) + Z__db[2];

	// Step 7
	double Y_eI__db = f_theta_h * Y_q__db;
	double Y_eI_10__db = f_theta_h * Y_10__db;

	// A_Y "is used to prevent available signal powers from exceeding levels expected for free-space propagation by an unrealistic
	//      amount when the variability about L_b(0.5) is large and L_b(0.5) is near its free-space level" [ES-83-3, p3-4]

	double A_YI = (A_T + Y_eI_10__db) - 3.0;

	*A_Y = MAX(A_YI, 0);

	*Y_e__db = Y_eI__db - *A_Y;

	// For variabilities less than 10%, do a correction check
	if (time_percentage < 0.10)
	{
		double c_Y[4] = { -5.0, -4.5, -3.7, 0.0 };
		for (int i = 1; i < 4; i++)
		{
			if (time_percentage <= data::P[i])
			{
				*Y_e__db += A_T;

				double c_Yi = LinearInterpolation(data::P[i - 1], c_Y[i - 1], data::P[i], c_Y[i], time_percentage);

				if (*Y_e__db > -c_Yi)
					*Y_e__db = -c_Yi;

				*Y_e__db -= A_T;

				break;
			}
		}
	}
}
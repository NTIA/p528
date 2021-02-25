#include "..\include\LOSTable.h"
#include "..\include\p528.h"
#include <math.h>

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                May 2019 : Geneva Study Group 3 Meetings
 |
 *===========================================================================*/

// Data Tables
double DATA_psi[140], DATA_delta_r[140], DATA_d[140];
double X_psi[55], X_delta_r[55], X_d[55];
double Y_psi[55], Y_delta_r[55], Y_d[55];
double Z_psi[55], Z_delta_r[55], Z_d[55];
double temp_psi[110], temp_delta_r[110], temp_d[110];
int i_X, i_Y, i_Z, i_DATA, i_temp;

double R[10] = { 0.06, 0.1, 1 / 9.0, 1 / 8.0, 1 / 7.0, 1 / 6.0, 1 / 5.0, 1 / 4.0, 1 / 3.0, 1 / 2.0 };
double degrees[24] = { .2, .5, .7, 1., 1.2, 1.5, 1.7, 2., 2.5, 3., 3.5, 4., 5., 6., 7., 8., 10., 20., 45., 70., 80., 85., 88., 89. };

void LOSTable::Build(Path path, Terminal terminal_1, Terminal terminal_2, double lambda)
{
	double RAD = PI / 180.0;

	i_Y = 0;
	i_X = 0;
	i_Z = 0;
	i_DATA = 0;

	Y_psi[i_Y] = 0.0;
	Y_delta_r[i_Y] = 0.0;
	Y_d[i_Y] = path.d_ML__km;
	i_Y++;

	for (int i = 0; i < 10; i++)
	{
		double psi = asin(lambda * R[i] / (2 * terminal_1.h__km));

		LineOfSightParams result;
		RayOptics(path, terminal_1, terminal_2, psi, &result);

		X_psi[i_X] = psi;
		X_delta_r[i_X] = result.delta_r;
		X_d[i_X] = result.d__km;
		i_X++;
	}

	for (int i = 0; i < 10; i++)
	{
		double psi = sqrt(lambda * R[i] / (2 * terminal_1.d__km));

		LineOfSightParams result;
		RayOptics(path, terminal_1, terminal_2, psi, &result);

		Y_psi[i_Y] = psi;
		Y_delta_r[i_Y] = result.delta_r;
		Y_d[i_Y] = result.d__km;
		i_Y++;
	}

	for (int i = 0; i < 24; i++)
	{
		double psi = degrees[i] * RAD;

		LineOfSightParams result;
		RayOptics(path, terminal_1, terminal_2, psi, &result);

		Z_psi[i_Z] = psi;
		Z_delta_r[i_Z] = result.delta_r;
		Z_d[i_Z] = result.d__km;
		i_Z++;
	}

	Z_psi[i_Z] = PI / 2;
	Z_delta_r[i_Z] = 2 * terminal_1.h__km;
	Z_d[i_Z] = 0.0;
	i_Z++;

	// Build lookup table
	ThreeColumnMergeSort(X_psi, X_delta_r, X_d, i_X, Y_psi, Y_delta_r, Y_d, i_Y, temp_psi, temp_delta_r, temp_d, &i_temp);
	ThreeColumnMergeSort(temp_psi, temp_delta_r, temp_d, i_temp, Z_psi, Z_delta_r, Z_d, i_Z, DATA_psi, DATA_delta_r, DATA_d, &i_DATA);

	double DATA_psis[140], DATA_delta_rs[140], DATA_ds[140];
	for (int i = 0; i < 140; i++)
	{
		DATA_psis[i] = DATA_psi[i];
		DATA_delta_rs[i] = DATA_delta_r[i];
		DATA_ds[i] = DATA_d[i];
	}
}

double LOSTable::GetDistanceFromTable(double delta_r)
{
	if (delta_r < DATA_delta_r[0])
		return DATA_d[0];
	else if (delta_r == DATA_delta_r[0])
		return DATA_d[0];
	else
	{
		for (int K = 1; K <= i_DATA; K++)
		{
			if (delta_r < DATA_delta_r[K])
			{
				return ((delta_r - DATA_delta_r[K - 1]) * (DATA_d[K] - DATA_d[K - 1]) / (DATA_delta_r[K] - DATA_delta_r[K - 1])) + DATA_d[K - 1];
			}
			else if (delta_r == DATA_delta_r[K])
			{
				return DATA_d[K];
			}
		}

		return DATA_d[i_DATA - 1];
	}
}

double LOSTable::GetPsiFromTable(double d__km)
{
	if (d__km > DATA_d[0])
		return DATA_psi[0];
	else if (d__km == DATA_d[0])
		return DATA_psi[0];
	else
	{
		for (int K = 1; K <= i_DATA; K++)
		{
			if (d__km > DATA_d[K])
				return ((d__km - DATA_d[K - 1]) * (DATA_psi[K] - DATA_psi[K - 1]) / (DATA_d[K] - DATA_d[K - 1])) + DATA_psi[K - 1];
			else if (d__km == DATA_d[K])
				return DATA_psi[K];
		}

		return DATA_psi[i_DATA - 1];
	}
}

void LOSTable::ThreeColumnMergeSort(double A_psi[], double A_delta_r[], double A_d[], int i_A,
	double B_psi[], double B_delta_r[], double B_d[], int i_B,
	double C_psi[], double C_delta_r[], double C_d[], int *i_C)
{
	int I = 1;
	int J = 1;
	*i_C = 0;

	while (I <= i_A && J <= i_B)
	{
		*i_C = *i_C + 1;
		// merge arrays, sorting on psi

		if (A_psi[I - 1] < B_psi[J - 1])
		{
			// Stick A_psi into resulting C_psi array
			C_psi[*i_C - 1] = A_psi[I - 1];
			C_delta_r[*i_C - 1] = A_delta_r[I - 1];
			C_d[*i_C - 1] = A_d[I - 1];
			I = I + 1;

			if (I <= i_A)
				continue; // still more psi in A_psi
			else if (A_psi[I - 1] == B_psi[J - 1])
			{
				// Stick A_psi into resulting C_psi array, but increment both A and B since they are equal(remove doubles when merging)
				C_psi[*i_C - 1] = A_psi[I - 1];
				C_delta_r[*i_C - 1] = A_delta_r[I - 1];
				C_d[*i_C - 1] = A_d[I - 1];
				I = I + 1;
				J = J + 1;

				if (I <= i_A)
				{
					// there are still more A_psi values
					if (J <= i_B)
						continue; //there are still more B_psi values
					else
					{
						// there are no more B_psi values, add all remaining A_psi values
						for (int k = I; k <= i_A; k++)
						{
							*i_C = *i_C + 1;
							C_psi[*i_C - 1] = A_psi[k - 1];
							C_delta_r[*i_C - 1] = A_delta_r[k - 1];
							C_d[*i_C - 1] = A_d[k - 1];
						}
					}
				}
				else
				{
					if (J > i_B)
						return; // All A_psi and B_psi values have been added
				}
			}
		}
		else
		{
			// Stick B_psi into resulting C_psi array
			C_psi[*i_C - 1] = B_psi[J - 1];
			C_delta_r[*i_C - 1] = B_delta_r[J - 1];
			C_d[*i_C - 1] = B_d[J - 1];
			J = J + 1;

			if (J <= i_B)
				continue; // there are still more B_psi values
			else
			{
				// there are no more B_psi values, add all remaining A_psi values
				for (int k = I; k <= i_A; k++)
				{
					*i_C = *i_C + 1;
					C_psi[*i_C - 1] = A_psi[k - 1];
					C_delta_r[*i_C - 1] = A_delta_r[k - 1];
					C_d[*i_C - 1] = A_d[k - 1];
				}
			}
		}

		// All A_psi values are added, so add the remaining B_psi values
		for (int k = J; k <= i_B; k++)
		{
			*i_C = *i_C + 1;
			C_psi[*i_C - 1] = B_psi[k - 1];
			C_delta_r[*i_C - 1] = B_delta_r[k - 1];
			C_d[*i_C - 1] = B_d[k - 1];
		}
	}
}

LOSTable::LOSTable()
{
}

LOSTable::~LOSTable()
{
}

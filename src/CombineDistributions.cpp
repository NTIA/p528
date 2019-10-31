#include <math.h>
#include "../include/p528.h"

/*=============================================================================
 |
 |  Description:  This function combines two distributions A and B, returning
 |                the resulting percentile.
 |
 |        Input:  A_M   - Mean of distribution A
 |                A_q   - q% of distribution A
 |                B_M   - Mean of distribution B
 |                B_q   - q% of distribution B
 |                q     - Quantile
 |
 |       Returns: C_q   - q% of resulting distribution C
 |
 *===========================================================================*/
double CombineDistributions(double A_M, double A_q, double B_M, double B_q, double q)
{
    double C_M = A_M + B_M;

    double Y_1, Y_2, Y_3;

    Y_1 = A_q - A_M;
    Y_2 = B_q - B_M;

    Y_3 = sqrt(pow(Y_1, 2) + pow(Y_2, 2));

    if (q < 0.5)
        return C_M + Y_3;
    else
        return C_M - Y_3;
}
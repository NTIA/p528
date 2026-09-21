#include <math.h>
#include "p528.h"

/********************************************************************************
 * Combines two distributions A and B, returning the resulting percentile.
 *
 * @param[in] A_M   Mean of distribution A
 * @param[in] A_p   p% of distribution A
 * @param[in] B_M   Mean of distribution B
 * @param[in] B_p   p% of distribution B
 * @param[in] p     Percentage
 * @returns         p% of resulting distribution C
 *******************************************************************************/
double CombineDistributions(double A_M, double A_p, double B_M, double B_p, double p)
{
    double C_M = A_M + B_M;

    double Y_1, Y_2, Y_3;

    Y_1 = A_p - A_M;
    Y_2 = B_p - B_M;

    Y_3 = sqrt(pow(Y_1, 2) + pow(Y_2, 2));

    if (p < 50)
        return C_M + Y_3;
    else
        return C_M - Y_3;
}

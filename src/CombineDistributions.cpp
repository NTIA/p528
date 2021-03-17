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
 |  Description:  This function combines two distributions A and B, returning
 |                the resulting percentile.
 |
 |        Input:  A_M   - Mean of distribution A
 |                A_p   - p% of distribution A
 |                B_M   - Mean of distribution B
 |                B_p   - p% of distribution B
 |                p     - Percentage
 |
 |       Returns: C_p   - p% of resulting distribution C
 |
 *===========================================================================*/
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
#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  This function computes the value of the Nakagami-Rice
 |                distribution for K and q%
 |
 |        Input:  K         - K-value
 |                q         - Time percentage
 |
 |      Outputs:  Y_pi__db  - Variability, in dB
 |
 *===========================================================================*/
double NakagamiRice(double K, double q)
{
    int K_INDEX = 0;

    // find first value greater than K
    for (int i = 1; i < 14; i++)
    {
        if (K <= data::NR_Data[i][K_INDEX])
        {
            // find first P value greater than q
            for (int j = 0; j < data::NUM_OF_PROBS; j++)
            {
                if (q <= data::P[j])
                {
                    // interpolate in K first
                    double VF_i = LinearInterpolation(data::NR_Data[i - 1][K_INDEX], data::NR_Data[i - i][j - 1 + 1], data::NR_Data[i][0], data::NR_Data[i][j - 1 + 1], K);
                    double VF_ii = LinearInterpolation(data::NR_Data[i - 1][K_INDEX], data::NR_Data[i - 1][j + 1], data::NR_Data[i][0], data::NR_Data[i][j + 1], K);

                    // interpolate in q
                    return LinearInterpolation(data::P[j - 1], VF_i, data::P[j], VF_ii, q);
                }
            }
        }
    }

    // find first P value greater than q
    int K_LIMIT = 13;
    for (int j = 0; j < data::NUM_OF_PROBS; j++)
    {
        if (q <= data::P[j])
            return LinearInterpolation(data::P[j - 1], data::NR_Data[K_LIMIT][j - 1 + 1], data::P[j], data::NR_Data[K_LIMIT][j + 1], q);
    }
}
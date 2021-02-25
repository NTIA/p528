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

	// search through stored values of K...
	for (int i_K = 1; i_K < data::NUM_OF_PROBS; i_K++)
	{
        // ...and find first value greater than input value K
		if (K <= data::NR_Data[i_K][K_INDEX])
		{
			// find first P value greater than q (start at index 2 instead of 1 cause index 0 is K values)
            // search through values of P...
			for (int i_P = 1; i_P < data::NUM_OF_PROBS; i_P++)
			{
                // ...and find first value greater than input value q
				if (q <= data::P[i_P])
				{
					// interpolate in K first
					double VF_low = LinearInterpolation(data::NR_Data[i_K - 1][K_INDEX], data::NR_Data[i_K - 1][i_P], data::NR_Data[i_K][K_INDEX], data::NR_Data[i_K][i_P], K);
					double VF_high = LinearInterpolation(data::NR_Data[i_K - 1][K_INDEX], data::NR_Data[i_K - 1][i_P + 1], data::NR_Data[i_K][K_INDEX], data::NR_Data[i_K][i_P + 1], K);
					
                    // interpolate in q
					return LinearInterpolation(data::P[i_P - 1], VF_low, data::P[i_P], VF_high, q);
				}
			}
		}
	}

	// find first P value greater than q (start at index 2 instead of 1 cause index 0 is K values)
	int K_LIMIT = data::NUM_OF_PROBS - 1;
	for (int i_P = 1; i_P < data::NUM_OF_PROBS; i_P++)
	{
		if (q <= data::P[i_P])
			return LinearInterpolation(data::P[i_P - 1], data::NR_Data[K_LIMIT][i_P], data::P[i_P], data::NR_Data[K_LIMIT][i_P + 1], q);
	}
} 
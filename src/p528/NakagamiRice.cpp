#include "..\..\include\p528.h"

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
 |                distribution for K and p%
 |
 |        Input:  K         - K-value
 |                p         - Time percentage
 |
 |      Outputs:  Y_pi__db  - Variability, in dB
 |
 *===========================================================================*/
double NakagamiRice(double K, double p)
{
    auto lower_K = lower_bound(data::K.begin(), data::K.end(), K);
    auto d_K = distance(data::K.begin(), lower_K);

    auto lower_p = lower_bound(data::P.begin(), data::P.end(), p);
    auto d_p = distance(data::P.begin(), lower_p);

    if (d_K == 0) // K <= -40
    {
        if (d_p == 0)
            return data::NakagamiRiceCurves[0][0];
        else
            return LinearInterpolation(data::P[d_p], data::NakagamiRiceCurves[0][d_p], data::P[d_p - 1], data::NakagamiRiceCurves[0][d_p - 1], p);
    }
    else if (d_K == data::K.size()) // K > 20
    {
        if (d_p == 0)
            return data::NakagamiRiceCurves[d_K - 1][0];
        else
            return LinearInterpolation(data::P[d_p], data::NakagamiRiceCurves[d_K - 1][d_p], data::P[d_p - 1], data::NakagamiRiceCurves[d_K - 1][d_p - 1], p);
    }
    else
    {
        if (d_p == 0)
            return LinearInterpolation(data::K[d_K], data::NakagamiRiceCurves[d_K][0],
                data::K[d_K - 1], data::NakagamiRiceCurves[d_K - 1][0], K);
        else
        {
            // interpolate between K's at constant p first
            double v1 = LinearInterpolation(data::K[d_K], data::NakagamiRiceCurves[d_K][d_p],
                data::K[d_K - 1], data::NakagamiRiceCurves[d_K - 1][d_p], K);
            double v2 = LinearInterpolation(data::K[d_K], data::NakagamiRiceCurves[d_K][d_p - 1],
                data::K[d_K - 1], data::NakagamiRiceCurves[d_K - 1][d_p] - 1, K);

            return LinearInterpolation(data::P[d_p], v1, data::P[d_p - 1], v2, p);
        }
    }
}
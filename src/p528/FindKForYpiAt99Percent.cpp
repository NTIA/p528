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
 |  Description:  This function returns the K-value of the Nakagami-Rice
 |                distribution for the given value of Y_pi(99)
 |
 |        Input:  Y_pi_99__db   - Y_pi(99), in dB
 |
 |       Returns: K             - K-value
 |
 *===========================================================================*/
double FindKForYpiAt99Percent(double Y_pi_99__db)
{
    // is Y_pi_99__db smaller than the smallest value in the distribution data
    if (Y_pi_99__db < data::NakagamiRiceCurves.front()[Y_pi_99_INDEX])
        return data::K.front();

    // search the distribution data and interpolate to find K (dependent variable)
    for (int i = 0; i < data::K.size(); i++)
        if (Y_pi_99__db - data::NakagamiRiceCurves[i][Y_pi_99_INDEX] < 0)
            return (data::K[i] * (Y_pi_99__db - data::NakagamiRiceCurves[i - 1][Y_pi_99_INDEX]) - data::K[i - 1] * (Y_pi_99__db - data::NakagamiRiceCurves[i][Y_pi_99_INDEX])) / (data::NakagamiRiceCurves[i][Y_pi_99_INDEX] - data::NakagamiRiceCurves[i - 1][Y_pi_99_INDEX]);

    // no match.  Y_pi_99__db is greater than the data contains.  Return largest K
    return data::K.back();
}
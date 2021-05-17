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
 |        Input:  Y_pi_99__db   - Y_pi(0.99), in dB
 |
 |       Returns: K             - K-value
 |
 *===========================================================================*/
double FindKForYpiAt99Percent(double Y_pi_99__db)
{
    int i = -1;

    do
    {
        i++;

        if (Y_pi_99__db - data::NR_Data[i][17] < 0)
            break;
        else if (Y_pi_99__db - data::NR_Data[i][17] == 0)
            return data::NR_Data[i][0];
    } while (i < 16);

    if (i == 0)
        return data::NR_Data[0][0];
    
    // Interpolate to find K (dependent variable)
    return (data::NR_Data[i][0] * (Y_pi_99__db - data::NR_Data[i - 1][17]) - data::NR_Data[i - 1][0] * (Y_pi_99__db - data::NR_Data[i][17])) / (data::NR_Data[i][17] - data::NR_Data[i - 1][17]);
}
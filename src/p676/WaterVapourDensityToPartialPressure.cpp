#include "..\..\include\p676.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 *===========================================================================*/

double WaterVapourDensityToPartialPressure(double rho__g_m3, double T__kelvin)
{
    // Equation 4
    double e__hPa = (rho__g_m3 * T__kelvin) / 216.7;

    return e__hPa;
}
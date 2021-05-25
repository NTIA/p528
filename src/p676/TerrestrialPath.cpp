#include "..\..\include\p676.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Gaseous attenuation for a terrestrial path, or for slightly
 |                inclined paths close to the ground.
 |
 |        Input:  f__ghz        - Frequency, in GHz
 |                T__kelvin     - Temperature, in Kelvin
 |                e__hPa        - Water vapour partial pressure, in hPa
 |                p__hPa        - Dry air pressure, in hPa
 |                r_0__km       - Path length, in km
 |
 |      Returns:  A__db         - Gaseous attenuation, in dB
 |
 *===========================================================================*/
double TerrestrialPath(double f__ghz, double T__kelvin, double e__hPa, double p__hPa, double r_0__km)
{
    double gamma = SpecificAttenuation(f__ghz, T__kelvin, e__hPa, p__hPa);

    // Equation 10
    double A__db = gamma * r_0__km;

    return A__db;
}
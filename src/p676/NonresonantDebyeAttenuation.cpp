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
 |  Description:  Non-resonant Debye component of frequency-dependent complex
 |                refractivity.
 |
 |        Input:  f__ghz        - Frequency, in GHz
 |                e__hPa        - Water vapour partial pressure, in hPa
 |                p__hPa        - Dry air pressure, in hPa
 |                theta         - From Equation 3
 |
 |      Returns:  N_D           - Non-resonant Debye component
 |
 *===========================================================================*/
double NonresonantDebyeAttenuation(double f__ghz, double e__hPa, double p__hPa, double theta)
{
    // width parameter for the Debye spectrum, Equation 9
    double d = 5.6e-4 * (p__hPa + e__hPa) * pow(theta, 0.8);

    // Equation 8
    double frac_1 = 6.14e-5 / (d * (1 + pow(f__ghz / d, 2)));
    double frac_2 = (1.4e-12 * p__hPa * pow(theta, 1.5)) / (1 + 1.9e-5 * pow(f__ghz, 1.5));
    double N_D = f__ghz * p__hPa * pow(theta, 2) * (frac_1 + frac_2);

    return N_D;
}
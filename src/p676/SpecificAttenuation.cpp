#include "../../include/p676.h"

/*=============================================================================
 |
 |  Description:  The specific gaseous attenuation due to dry air and 
 |                water vapour, in dB/km.
 |
 |        Input:  f__ghz        - Frequency, in GHz
 |                T__kelvin     - Temperature, in Kelvin
 |                e__hPa        - Water vapour partial pressure, in hPa
 |                p__hPa        - Dry air pressure, in hPa
 |
 |      Returns:  gamma         - Specific gaseous attenuation, in dB/km
 |
 *===========================================================================*/
double SpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double p__hPa)
{
    double gamma_o = OxygenSpecificAttenuation(f__ghz, T__kelvin, e__hPa, p__hPa);
    double gamma_w = WaterVapourSpecificAttenuation(f__ghz, T__kelvin, e__hPa, p__hPa);

    double gamma = gamma_o + gamma_w;   // [Eqn 1]

    return gamma;
}

/*=============================================================================
 |
 |  Description:  The oxygen specific gaseous attenuation due to dry 
 |                air, in dB/km.
 |
 |        Input:  f__ghz        - Frequency, in GHz
 |                T__kelvin     - Temperature, in Kelvin
 |                e__hPa        - Water vapour partial pressure, in hPa
 |                p__hPa        - Dry air pressure, in hPa
 |
 |      Returns:  gamma_o       - Oxygen specific gaseous attenuation, 
 |                                in dB/km
 |
 *===========================================================================*/
double OxygenSpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double p__hPa)
{
    // partial Eqn 1
    double N_o = OxygenRefractivity(f__ghz, T__kelvin, e__hPa, p__hPa);
    double gamma_o = 0.1820 * f__ghz * N_o;

    return gamma_o;
}

/*=============================================================================
 |
 |  Description:  The water vapour specific gaseous attenuation, in dB/km.
 |
 |        Input:  f__ghz        - Frequency, in GHz
 |                T__kelvin     - Temperature, in Kelvin
 |                e__hPa        - Water vapour partial pressure, in hPa
 |                p__hPa        - Dry air pressure, in hPa
 |
 |      Returns:  gamma_w       - Specific gaseous attenuation due to water
 |                                vapour, in dB/km
 |
 *===========================================================================*/
double WaterVapourSpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double p__hPa)
{
    // partial Eqn 1
    double N_w = WaterVapourRefractivity(f__ghz, T__kelvin, e__hPa, p__hPa);
    double gamma_w = 0.1820 * f__ghz * N_w;

    return gamma_w;
}
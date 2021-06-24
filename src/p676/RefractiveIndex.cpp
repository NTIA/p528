#include "../../include/p676.h"

/*=============================================================================
 |
 |  Description:  Compute the refractive index.
 |
 |        Input:  p__hPa        - Dry pressure, in hPa
 |                T__kelvin     - Temperature, in Kelvin
 |                e__hPa        - Water vapour pressure, in hPa
 |
 |      Returns:  n             - Refractive index
 |
 *===========================================================================*/
double RefractiveIndex(double p__hPa, double T__kelvin, double e__hPa)
{
    // dry term of refractivity
    double N_dry = 77.6 * p__hPa / T__kelvin;

    // wet term of refractivity
    double N_wet = 72 * e__hPa / T__kelvin + 3.75e5 * e__hPa / pow(T__kelvin, 2);

    double N = N_dry + N_wet;

    double n = 1 + N * pow(10, -6);

    return n;
}

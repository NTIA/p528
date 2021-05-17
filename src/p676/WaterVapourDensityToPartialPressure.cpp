#include "..\..\include\p676.h"

double WaterVapourDensityToPartialPressure(double rho__g_m3, double T__kelvin)
{
    // Equation 4
    double e__hPa = (rho__g_m3 * T__kelvin) / 216.7;

    return e__hPa;
}
#include "../../include/p835.h"

/*=============================================================================
 |
 |  Description:  Converts from geometric height, in km, to geopotential
 |                height, in km'.  See Equation (1a).
 |
 |        Input:  k__km         - Geometric height, in km
 |
 |      Returns:  k_prime__km   - Geopotential height, in km'
 |
 *===========================================================================*/
double ConvertToGeopotentialHeight(double h__km)
{
    return (6356.766 * h__km) / (6356.766 + h__km);
}

/*=============================================================================
 |
 |  Description:  Converts from geopotential height, in km', to geometric
 |                height, in km.  See Equation (1b).
 |
 |        Input:  k_prime__km   - Geopotential height, in km'
 |
 |      Returns:  k__km         - Geometric height, in km
 |
 *===========================================================================*/
double ConvertToGeometricHeight(double h_prime__km)
{
    return (6356.766 * h_prime__km) / (6356.766 - h_prime__km);
}

/*=============================================================================
 |
 |  Description:  Converts water vapour density, in g/m^3, to water vapour
 |                pressure, in hPa.  See Equation (8).
 |
 |        Input:  rho       - Water vapour density, rho(h), in g/m^3
 |                T__kelvin - Temperature, T(h), in Kelvin
 |
 |      Returns:  e__hPa    - Water vapour pressure, e(h), in hPa
 |
 *===========================================================================*/
double WaterVapourDensityToPressure(double rho, double T__kelvin)
{
    return (rho * T__kelvin) / 216.7;
}
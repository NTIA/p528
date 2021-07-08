#include "../../include/p676.h"
#include "../../include/p835.h"

/*=============================================================================
 |
 |  Description:  This file computes Step 6 in Annex 2, Section 3 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  h__km             - Geometric height, in km
 |
 |      Returns:  e__hPa            - Water vapour pressure, in hPa
 |
 *===========================================================================*/

double GlobalWetPressure(double h__km)
{
    double T__kelvin = GlobalTemperature(h__km);
    double P__hPa = GlobalPressure(h__km);
    double rho__g_m3 = std::max(GlobalWaterVapourDensity(h__km, RHO_0__M_KG), 2 * pow(10, -6) * 216.7 * P__hPa / T__kelvin);
    double e__hPa = WaterVapourDensityToPressure(rho__g_m3, T__kelvin);

    return e__hPa;
}
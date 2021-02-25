#include "math.h"
#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric temperature,
 |                in Kelvin.
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  T__kelvin     - Temperature, in Kelvin.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalTemperature(double h__km)
{
    if (h__km < 86)
    {
        double h_prime__km = ConvertToGeopotentialHeight(h__km);
        return GlobalTemperature_Regime1(h_prime__km);
    }
    else
        return GlobalTemperature_Regime2(h__km);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric temperature,
 |                in Kelvin, for the first height regime.
 |                See Equations (2a-g).
 |
 |        Input:  h_prime__km   - Geopotential height, in km'
 |
 |      Returns:  T__kelvin     - Temperature, in Kelvin.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalTemperature_Regime1(double h_prime__km)
{
    if (h_prime__km <= 11)
        return 288.15 - 6.5 * h_prime__km;
    else if (h_prime__km <= 20)
        return 216.65;
    else if (h_prime__km <= 32)
        return 216.65 + (h_prime__km - 20);
    else if (h_prime__km <= 47)
        return 228.65 + 2.8 * (h_prime__km - 32);
    else if (h_prime__km <= 51)
        return 270.65;
    else if (h_prime__km <= 71)
        return 270.65 - 2.8 * (h_prime__km - 51);
    else
        return 214.65 - 2.0 * (h_prime__km - 71);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric temperature,
 |                in Kelvin, for the second height regime.
 |                See Equations (4a-b).
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  T__kelvin     - Temperature, in Kelvin.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalTemperature_Regime2(double h__km)
{
    if (h__km <= 91)
        return 186.8673;
    else
        return 263.1905 - 76.3232 * sqrt(1 - pow((h__km - 91) / 19.9429, 2));
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric pressure,
 |                in hPa.
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  P__hPa        - Pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalPressure(double h__km)
{
    if (h__km < 86)
    {
        double h_prime__km = ConvertToGeopotentialHeight(h__km);
        return GlobalPressure_Regime1(h_prime__km);
    }
    else
        return GlobalPressure_Regime2(h__km);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric pressure,
 |                in hPa, for the first height regime.  See Equations (3a-g).
 |
 |        Input:  h_prime__km   - Geopotential height, in km'
 |
 |      Returns:  P__hPa        - Pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalPressure_Regime1(double h_prime__km)
{
    if (h_prime__km <= 11)
        return 1013.25 * pow(288.15 / (288.15 - 6.5 * h_prime__km), -34.1632 / 6.5);
    else if (h_prime__km <= 20)
        return 226.3226 * exp(-34.1632 * (h_prime__km - 11) / 216.65);
    else if (h_prime__km <= 32)
        return 54.74980 * pow(216.65 / (216.65 + (h_prime__km - 20)), 34.1632);
    else if (h_prime__km <= 47)
        return 8.680422 * pow(228.65 / (228.65 + 2.8 * (h_prime__km - 32)), 34.1632 / 2.8);
    else if (h_prime__km <= 51)
        return 1.109106 * exp(-34.1632 * (h_prime__km - 47) / 270.65);
    else if (h_prime__km <= 71)
        return 0.6694167 * pow(270.65 / (270.65 - 2.8 * (h_prime__km - 51)), -34.1632 / 2.8);
    else
        return 0.03956649 * pow(214.65 / (214.65 - 2.0 * (h_prime__km - 71)), -34.1632 / 2.0);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric pressure,
 |                in hPa, for the second height regime.  See Equation (5).
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  P__hPa        - Pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalPressure_Regime2(double h__km)
{
    double a_0 = 95.571899;
    double a_1 = -4.011801;
    double a_2 = 6.424731e-2;
    double a_3 = -4.789660e-4;
    double a_4 = 1.340543e-6;

    return exp(a_0 + a_1 * h__km + a_2 * pow(h__km, 2) + a_3 * pow(h__km, 3) + a_4 * pow(h__km, 4));
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric water vapour
 |                density, in g/m^3.  See Equation (6).
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  rho           - Density, in g/m^3.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalWaterVapourDensity(double h__km)
{
    double h_0__km = 2;     // scale height
    double rho_0 = 7.5;     // g/m^3

    return rho_0 * exp(-h__km / h_0__km);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric water vapour
 |                pressure, in hPa.
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  P__hPa        - Pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalWaterVapourPressure(double h__km)
{
    double rho = GlobalWaterVapourDensity(h__km);

    double T;
    if (h__km < 86)
    {
        // convert to geopotential height
        double h_prime__km = ConvertToGeopotentialHeight(h__km);
        T = GlobalTemperature_Regime1(h_prime__km);
    }
    else
        T = GlobalTemperature_Regime2(h__km);

    return WaterVapourDensityToPressure(rho, T);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric dry gas
 |                density, in g/m^3.
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  rho           - Density, in g/m^3.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalDryAtmosphereDensity(double h__km)
{
    double h_0__km = 6;     // scale height
    double rho_0 = 7.5;     // g/m^3

    return rho_0 * exp(-h__km / h_0__km);
}

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
 |  Description:  Converts water vapour density, in g/m^3, to water vapour
 |                pressure, in hPa.  See Equation (8).
 |
 |        Input:  rho       - Water vapour density, rho(h), in g/m^3
 |                T__kelvin - Temperature, T(h), in Kelvin
 |
 |      Returns:  e         - Water vapour pressure, e(h), in hPa
 |
 *===========================================================================*/
double WaterVapourDensityToPressure(double rho, double T__kelvin)
{
    return (rho * T__kelvin) / 216.7;
}
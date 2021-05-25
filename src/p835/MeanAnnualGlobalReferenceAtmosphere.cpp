#include "math.h"
#include "..\..\include\p835.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 *===========================================================================*/

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
    if (h__km < 0)
        return ERROR_HEIGHT_TOO_SMALL;
    if (h__km > 100)
        return ERROR_HEIGHT_TOO_LARGE;

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
    if (h_prime__km < 0)
        return ERROR_HEIGHT_TOO_SMALL;
    else if (h_prime__km <= 11)
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
    else if (h_prime__km <= 84.852)
        return 214.65 - 2.0 * (h_prime__km - 71);
    else
        return ERROR_HEIGHT_TOO_LARGE;
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
    if (h__km < 86)
        return ERROR_HEIGHT_TOO_SMALL;
    else if (h__km <= 91)
        return 186.8673;
    else if (h__km <= 100)
        return 263.1905 - 76.3232 * sqrt(1 - pow((h__km - 91) / 19.9429, 2));
    else
        return ERROR_HEIGHT_TOO_LARGE;
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric pressure,
 |                in hPa.
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  p__hPa        - Dry air pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalPressure(double h__km)
{
    if (h__km < 0)
        return ERROR_HEIGHT_TOO_SMALL;
    if (h__km > 100)
        return ERROR_HEIGHT_TOO_LARGE;

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
 |      Returns:  p__hPa        - Dry air pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalPressure_Regime1(double h_prime__km)
{
    if (h_prime__km < 0)
        return ERROR_HEIGHT_TOO_SMALL;
    else if (h_prime__km <= 11)
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
    else if (h_prime__km <= 84.852)
        return 0.03956649 * pow(214.65 / (214.65 - 2.0 * (h_prime__km - 71)), -34.1632 / 2.0);
    else
        return ERROR_HEIGHT_TOO_LARGE;
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric pressure, 
 |                in hPa, for the second height regime.  See Equation (5).
 |
 |        Input:  h__km         - Geometric height, in km
 |
 |      Returns:  p__hPa        - Dry air pressure, in hPa.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalPressure_Regime2(double h__km)
{
    if (h__km < 86)
        return ERROR_HEIGHT_TOO_SMALL;
    if (h__km > 100)
        return ERROR_HEIGHT_TOO_LARGE;
    
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
 |                rho_0         - Ground-level water vapour density, in g/m^3
 |
 |      Returns:  rho           - Water vapour density, in g/m^3.
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalWaterVapourDensity(double h__km, double rho_0)
{
    if (h__km < 0)
        return ERROR_HEIGHT_TOO_SMALL;
    if (h__km > 100)
        return ERROR_HEIGHT_TOO_LARGE;

    double h_0__km = 2;     // scale height

    return rho_0 * exp(-h__km / h_0__km);
}

/*=============================================================================
 |
 |  Description:  The mean annual global reference atmospheric water vapour 
 |                pressure, in hPa.
 |
 |        Input:  h__km         - Geometric height, in km
 |                rho_0         - Ground-level water vapour density, in g/m^3
 |
 |      Returns:  e__hPa        - Water vapour pressure, e(h), in hPa
 |                                Or error (negative number).
 |
 *===========================================================================*/
double GlobalWaterVapourPressure(double h__km, double rho_0)
{
    if (h__km < 0)
        return ERROR_HEIGHT_TOO_SMALL;
    if (h__km > 100)
        return ERROR_HEIGHT_TOO_LARGE;

    double rho = GlobalWaterVapourDensity(h__km, rho_0);

    double T__kelvin;
    if (h__km < 86)
    {
        // convert to geopotential height
        double h_prime__km = ConvertToGeopotentialHeight(h__km);
        T__kelvin = GlobalTemperature_Regime1(h_prime__km);
    }
    else
        T__kelvin = GlobalTemperature_Regime2(h__km);
    
    return WaterVapourDensityToPressure(rho, T__kelvin);
}
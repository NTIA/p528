#include "..\..\include\p676.h"
#include "..\..\include\p835.h"

double GlobalWetPressure(double h__km)
{
    double T__kelvin = GlobalTemperature(h__km);
    double P__hPa = GlobalPressure(h__km);
    double rho__g_m3 = MAX(GlobalWaterVapourDensity(h__km, RHO_0__M_KG), 2 * pow(10, -6) * 216.7 * P__hPa / T__kelvin);
    double e__hPa = WaterVapourDensityToPressure(rho__g_m3, T__kelvin);

    return e__hPa;
}
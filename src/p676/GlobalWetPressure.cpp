#include "..\..\include\p676.h"
#include "..\..\include\p835.h"

double GlobalWetPressure(double h__km)
{
    double T__kelvin = GlobalTemperature(h__km);
    double rho__g_m3 = GlobalWaterVapourDensity(h__km, RHO_0__M_KG);
    double e__hPa = WaterVapourDensityToPressure(rho__g_m3, T__kelvin);

    return e__hPa;
}
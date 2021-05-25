/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 *===========================================================================*/

#define DLLEXPORT extern "C" __declspec(dllexport)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

//
// CONSTANTS
///////////////////////////////////////////////

#define RHO_0__M_KG                        7.5

//
// ERROR CODES
///////////////////////////////////////////////

#define ERROR_HEIGHT_TOO_SMALL              -1
#define ERROR_HEIGHT_TOO_LARGE              -2

//
// FUNCTIONS
///////////////////////////////////////////////

DLLEXPORT double ConvertToGeopotentialHeight(double h__km);
DLLEXPORT double ConvertToGeometricHeight(double h_prime__km);
DLLEXPORT double WaterVapourDensityToPressure(double rho, double T__kelvin);

DLLEXPORT double GlobalTemperature(double h__km);
DLLEXPORT double GlobalTemperature_Regime1(double h_prime__km);
DLLEXPORT double GlobalTemperature_Regime2(double h__km);
DLLEXPORT double GlobalPressure(double h__km);
DLLEXPORT double GlobalPressure_Regime1(double h_prime__km);
DLLEXPORT double GlobalPressure_Regime2(double h__km);
DLLEXPORT double GlobalWaterVapourDensity(double h__km, double rho_0);
DLLEXPORT double GlobalWaterVapourPressure(double h__km, double rho_0);

DLLEXPORT double LowLatitudeTemperature(double h__km);
DLLEXPORT double LowLatitudePressure(double h__km);
DLLEXPORT double LowLatitudeWaterVapourDensity(double h__km);
DLLEXPORT double LowLatitudeWaterVapourPressure(double h__km);

DLLEXPORT double MidLatitudeSummerTemperature(double h__km);
DLLEXPORT double MidLatitudeSummerPressure(double h__km);
DLLEXPORT double MidLatitudeSummerWaterVapourDensity(double h__km);
DLLEXPORT double MidLatitudeSummerWaterVapourPressure(double h__km);

DLLEXPORT double MidLatitudeWinterTemperature(double h__km);
DLLEXPORT double MidLatitudeWinterPressure(double h__km);
DLLEXPORT double MidLatitudeWinterWaterVapourDensity(double h__km);
DLLEXPORT double MidLatitudeWinterWaterVapourPressure(double h__km);

DLLEXPORT double HighLatitudeSummerTemperature(double h__km);
DLLEXPORT double HighLatitudeSummerPressure(double h__km);
DLLEXPORT double HighLatitudeSummerWaterVapourDensity(double h__km);
DLLEXPORT double HighLatitudeSummerWaterVapourPressure(double h__km);

DLLEXPORT double HighLatitudeWinterTemperature(double h__km);
DLLEXPORT double HighLatitudeWinterPressure(double h__km);
DLLEXPORT double HighLatitudeWinterWaterVapourDensity(double h__km);
DLLEXPORT double HighLatitudeWinterWaterVapourPressure(double h__km);
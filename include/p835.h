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

double ConvertToGeopotentialHeight(double h__km);
double ConvertToGeometricHeight(double h_prime__km);
double WaterVapourDensityToPressure(double rho, double T__kelvin);

double GlobalTemperature(double h__km);
double GlobalTemperature_Regime1(double h_prime__km);
double GlobalTemperature_Regime2(double h__km);
double GlobalPressure(double h__km);
double GlobalPressure_Regime1(double h_prime__km);
double GlobalPressure_Regime2(double h__km);
double GlobalWaterVapourDensity(double h__km, double rho_0);
double GlobalWaterVapourPressure(double h__km, double rho_0);

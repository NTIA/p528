/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 *===========================================================================*/

#include <vector>
#include <algorithm>

using namespace std;

#define DLLEXPORT extern "C" __declspec(dllexport)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define PI                                  3.1415926535897932384
#define a_0__km                             6371.0

// Function pointers
using Temperature = double(*)(double);
using DryPressure = double(*)(double);
using WetPressure = double(*)(double);

struct SlantPathAttenuationResult
{
	double A_gas__db;						// 
	double bending__rad;					// 
	double a__km;							// 
	double angle__rad;						// 
	double delta_L__km;						// Excess atmospheric path length, in km
};

struct RayTraceConfig
{
	Temperature temperature;
	DryPressure dry_pressure;
	WetPressure wet_pressure;
};

class OxygenData
{
public:
	const static vector<double> f_0;		// 
	const static vector<double> a_1;		//
	const static vector<double> a_2;		//
	const static vector<double> a_3;		//
	const static vector<double> a_4;		//
	const static vector<double> a_5;		//
	const static vector<double> a_6;		//
};

class WaterVapourData
{
public:
	const static vector<double> f_0;		// 
	const static vector<double> b_1;		//
	const static vector<double> b_2;		//
	const static vector<double> b_3;		//
	const static vector<double> b_4;		//
	const static vector<double> b_5;		//
	const static vector<double> b_6;		//
};

double LineShapeFactor(double f__ghz, double f_i__ghz, double delta_f__ghz, double delta);
double NonresonantDebyeAttenuation(double f__ghz, double e__hPa, double p__hPa, double theta);
double RefractiveIndex(double p__hPa, double T__kelvin, double e__hPa);
void GetLayerProperties(double f__ghz, double h_i__km, RayTraceConfig config,
	double* n, double* gamma);

double SpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double p__hPa);
double OxygenRefractivity(double f__ghz, double T__kelvin, double e__hPa, double p__hPa);
double WaterVapourRefractivity(double f__ghz, double T__kelvin, double e__hPa, double p__hPa);
double OxygenSpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double P__hPa);
double WaterVapourSpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double p__hPa);
double WaterVapourDensityToPartialPressure(double rho__g_m3, double T__kelvin);

void RayTrace(double f__ghz, double h_1__km, double h_2__km, double beta_1__rad,
	RayTraceConfig config, SlantPathAttenuationResult* result);

int SlantPathAttenuation(double f__ghz, double h_1__km, double h_2__km, double beta_1__rad,
	SlantPathAttenuationResult* result);

double GlobalWetPressure(double h__km);
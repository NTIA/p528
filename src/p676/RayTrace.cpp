#include "..\..\include\p676.h"
#include "..\..\include\p835.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  Thickness of the ith layer.
 |
 |        Input:  m             - Internal parameter
 |                i             - Layer of interest
 |
 |      Returns:  delta_i__km   - Layer thickness, in km
 |
 *===========================================================================*/
double LayerThickness(double m, int i)
{
    // Equation 14
    double delta_i__km = m * exp((i - 1) / 100.);

    return delta_i__km;
}

void RayTrace(double f__ghz, double h_1__km, double h_2__km, double beta_1__rad,
    RayTraceConfig config, SlantPathAttenuationResult* result)
{
    // Equations 16(a)-(c)
    int i_lower = floor(100 * log(1e4 * h_1__km * (exp(1. / 100.) - 1) + 1) + 1);
    int i_upper = ceil(100 * log(1e4 * h_2__km * (exp(1. / 100.) - 1) + 1) + 1);
    double m = ((exp(2. / 100.) - exp(1. / 100.)) / (exp(i_upper / 100.) - exp(i_lower / 100.))) * (h_2__km - h_1__km);

    double gamma_i;
    double gamma_ii;
    double n_i;
    double n_ii;
    double r_i__km;
    double r_ii__km;
    double h_i__km;
    double h_ii__km;
    double a_i__km;
    double alpha_i__rad = beta_1__rad;
    double delta_i__km;
    double delta_ii__km;
    double beta_i__rad;
    double beta_ii__rad = beta_1__rad;

    // initialize results
    result->A_gas__db = 0;
    result->bending__rad = 0;
    result->a__km = 0;
    result->delta_L__km = 0;

    // initialize starting layer
    delta_i__km = LayerThickness(m, i_lower);
    h_i__km = h_1__km + m * ((exp((i_lower - 1) / 100.) - exp((i_lower - 1) / 100.)) / (exp(1 / 100.) - 1));
    GetLayerProperties(f__ghz, h_i__km + delta_i__km / 2, config, &n_i, &gamma_i);
    r_i__km = a_0__km + h_i__km;

    // record bottom layer properties for alpha and beta calculations
    double r_1__km = r_i__km;
    double n_1 = n_i;

    // summation from Equation 13
    for (int i = i_lower; i <= i_upper - 1; i++)
    {
        delta_ii__km = LayerThickness(m, i + 1);
        h_ii__km = h_1__km + m * ((exp((i + 1 - 1) / 100.) - exp((i_lower - 1) / 100.)) / (exp(1 / 100.) - 1));

        GetLayerProperties(f__ghz, h_ii__km + delta_ii__km / 2, config, &n_ii, &gamma_ii);

        r_ii__km = a_0__km + h_ii__km;

        delta_i__km = LayerThickness(m, i);

        // Equation 19b
        beta_i__rad = asin(MIN(1, (n_1 * r_1__km) / (n_i * r_i__km) * sin(beta_1__rad)));

        // entry angle into the layer interface, Equation 18a
        alpha_i__rad = asin(MIN(1, (n_1 * r_1__km) / (n_i * r_ii__km) * sin(beta_1__rad)));

        // path lenth through ith layer, Equation 17
        a_i__km = -r_i__km * cos(beta_i__rad) + sqrt(pow(r_i__km, 2) * pow(cos(beta_i__rad), 2) + 2 * r_i__km * delta_i__km + pow(delta_i__km, 2));

        result->a__km += a_i__km;
        result->A_gas__db += a_i__km * gamma_i;
        result->delta_L__km += a_i__km * (n_i - 1);     // summation, Equation 23

        beta_ii__rad = asin(n_i / n_ii * sin(alpha_i__rad));

        // summation of the bending angle, Equation 22a
        // the summation only goes to i_max - 1
        if (i != i_upper - 1)
            result->bending__rad += beta_ii__rad - alpha_i__rad;

        // shift for next loop
        h_i__km = h_ii__km;
        n_i = n_ii;
        gamma_i = gamma_ii;
        r_i__km = r_ii__km;
    }

    result->angle__rad = alpha_i__rad;
}

void GetLayerProperties(double f__ghz, double h_i__km, RayTraceConfig config,
    double* n, double* gamma)
{
    // use function pointers to get atmospheric parameters
    double T__kelvin = config.temperature(h_i__km);
    double p__hPa = config.dry_pressure(h_i__km);
    double e__hPa = config.wet_pressure(h_i__km);

    // compute the refractive index for the current layer
    *n = RefractiveIndex(p__hPa, T__kelvin, e__hPa);

    // specific attenuation of layer
    *gamma = SpecificAttenuation(f__ghz, T__kelvin, e__hPa, p__hPa);
}
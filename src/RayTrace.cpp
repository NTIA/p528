#include "math.h"
#include "..\include\p528.h"

double LayerThickness(double m, int i)
{
    double delta_i__km = m * exp((i - 1) / 100.);

    return delta_i__km;
}

double LayerBottom(double m, int i)
{
    double h_i__km = m * (exp((i - 1) / 100.) - 1) / (exp(1 / 100.) - 1);

    return h_i__km;
}

void RayTrace(double f__mhz, double h_tx__km, double h_rx__km, double theta_tx, double* d_arc__km, 
    double* theta_rx, double* A_a__db, double* a__km)
{
    double f__ghz = f__mhz / 1000;

    // P.676 Eqns 16(a)-(c)
    int i_lower = floor(100 * log(1e4 * h_tx__km * (exp(1. / 100.) - 1) + 1) + 1);// Eqn 16a
    int i_upper = ceil(100 * log(1e4 * h_rx__km * (exp(1. / 100.) - 1) + 1) + 1);
    double m = ((exp(2. / 100.) - exp(1. / 100.)) / (exp(i_upper / 100.) - exp(i_lower / 100.))) * (h_rx__km - h_tx__km);

    int i = i_lower;
    double delta_i__km = LayerThickness(m, i);         // thickness of current layer
    double delta_ii__km = LayerThickness(m, i + 1);    //
    double h_i__km = LayerBottom(m, i);                // height of the bottom of the layer
    double h_ii__km = LayerBottom(m, i + 1);           //

    double beta_i = PI / 2 - theta_tx;   // exit angle
    double alpha_i; // enter angle
    double a_i__km;     // path length
    double r_i__km; // bottom of layer
    double r_rx__km = a_0__km + h_rx__km;            		// radius of rx

    double h_i_mid__km, h_ii_mid__km;
    double T_i__kelvin;
    double P_d_i__hPa;
    double rho_i__g_m3;             // water vapour density, in g/m^3.
    double e__hPa;
    double n_i, n_ii;
    double gamma_i, gamma_ii;
    double beta_ii = beta_i;

    double tau = 0; // bending angle

    *A_a__db = 0; // total atmospheric absorption
    *a__km = 0;     // total ray path length, in km

    // initialize first layer
    h_i_mid__km = h_i__km + delta_i__km / 2;
    GetLayerProperties(f__ghz, h_i_mid__km, &n_i, &gamma_i);

    // loop as long as the full layer is below h_rx__km
    while (h_i__km + delta_i__km <= h_rx__km && i <= (i_upper - 1) || i == 1)
    {
        // radial to bottom of layer
        r_i__km = a_0__km + h_i__km;

        // compute entry angle into the layer
        alpha_i = asin((r_i__km / (r_i__km + delta_i__km)) * sin(beta_i));

        // compute the length of the path through the current layer
        a_i__km = -r_i__km * cos(beta_i) + sqrt(pow(r_i__km, 2) * pow(cos(beta_i), 2) + 2 * r_i__km * delta_i__km + pow(delta_i__km, 2));
        *a__km += a_i__km;

        // layer midpoint
        h_ii_mid__km = h_ii__km + delta_ii__km / 2;

        GetLayerProperties(f__ghz, h_ii_mid__km, &n_ii, &gamma_ii);

        *A_a__db += a_i__km * gamma_i;

        beta_ii = asin(n_i / n_ii * sin(alpha_i));

        // accumulate the bending angle
        tau += beta_ii - alpha_i;

        // shift params from iith layer to ith layer
        delta_i__km = delta_ii__km;
        h_i__km = h_ii__km;
        h_i_mid__km = h_ii_mid__km;
        beta_i = beta_ii;
        n_i = n_ii;
        gamma_i = gamma_ii;

        // compute next layer
        i++;
        delta_ii__km = LayerThickness(m, i + 1);
        h_ii__km = LayerBottom(m, i + 1);

        // check if need to use a partial layer
        if (h_i__km < h_rx__km && h_ii__km > h_rx__km)
        {
            // clamp to h_rx__km
            delta_i__km = h_rx__km - h_i__km;
            delta_ii__km = h_ii__km - h_rx__km;
            h_ii__km = h_rx__km;
        }
    }

    *theta_rx = PI / 2 - beta_ii;

    // check if above the atmosphere
    if (h_rx__km > h_i__km)
    {
        *theta_rx = acos(cos(*theta_rx) * n_i * r_i__km / r_rx__km);      // [Thayer, Equ 1], rearranged with n = 1

        // remaining path length
        a_i__km = -r_i__km * cos(beta_i) + sqrt(pow(r_i__km, 2) * pow(cos(beta_i), 2) + 2 * r_i__km * delta_i__km + pow(delta_i__km, 2));
        *a__km += a_i__km;
    }

    double central_angle = (*theta_rx - theta_tx + tau);            // [Thayer, Equ 2], rearranged
    *d_arc__km = a_0__km * central_angle;
}

void GetLayerProperties(double f__ghz, double h_mid__km, double* n, double* gamma)
{
    // call into P.835 to get atmospheric values
    double T__kelvin = GlobalTemperature(h_mid__km);
    double P_d__hPa = GlobalPressure(h_mid__km);
    double rho__g_m3 = GlobalWaterVapourDensity(h_mid__km);
    double e__hPa = WaterVapourDensityToPressure(rho__g_m3, T__kelvin);

    // compute the refractive index for the current layer
    *n = RefractiveIndex(P_d__hPa, T__kelvin, e__hPa);

    // specific attenuation of layer
    *gamma = SpecificAttenuation(f__ghz, T__kelvin, e__hPa, P_d__hPa);
}

double RefractiveIndex(double P_d__hPa, double T__kelvin, double e)
{
    // dry term of refractivity
    double N_dry = 77.6 * P_d__hPa / T__kelvin;

    // wet term of refractivity
    double N_wet = 72 * e / T__kelvin + 3.75e5 * e / pow(T__kelvin, 2);

    double N = N_dry + N_wet;

    double n = 1 + N * 1e-6;

    return n;
}
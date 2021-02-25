#include "..\include\p528.h"

double SpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double P__hPa)
{
    double gamma_o = OxygenSpecificAttenuation(f__ghz, T__kelvin, e__hPa, P__hPa);
    double gamma_w = WaterVapourSpecificAttenuation(f__ghz, T__kelvin, e__hPa, P__hPa);

    double gamma = gamma_o + gamma_w;

    return gamma;
}

double OxygenSpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double P__hPa)
{
    double N_o = OxygenRefractivity(f__ghz, T__kelvin, e__hPa, P__hPa);
    double gamma_o = 0.1820 * f__ghz * N_o;

    return gamma_o;
}

double WaterVapourSpecificAttenuation(double f__ghz, double T__kelvin, double e__hPa, double P__hPa)
{
    double N = WaterVapourRefractivity(f__ghz, T__kelvin, e__hPa, P__hPa);
    double gamma_w = 0.1820 * f__ghz * N;

    return gamma_w;
}

// Equation 2a
double OxygenRefractivity(double f__ghz, double T__kelvin, double e__hPa, double P__hPa)
{
    double theta = 300 / T__kelvin;

    double N = 0;

    for (int i = 0; i < OxygenData::f_0.size(); i++)
    {
        double S_i = OxygenData::a_1[i] * 1e-7 * P__hPa * pow(theta, 3) * exp(OxygenData::a_2[i] * (1 - theta));    // Equation 3

        // compute the width of the line
        double delta_f__ghz = OxygenData::a_3[i] * 1e-4 * (P__hPa * pow(theta, (0.8 - OxygenData::a_4[i])) + 1.1 * e__hPa * theta);

        // modify the line width to account for Zeeman splitting of the oxygen lines
        delta_f__ghz = sqrt(pow(delta_f__ghz, 2) + 2.25e-6);

        // correction factor due to interference effects in oxygen lines; Equation 7
        double delta = (OxygenData::a_5[i] + OxygenData::a_6[i] * theta) * 1e-4 * (P__hPa + e__hPa) * pow(theta, 0.8);

        double F_i = LineShapeFactor(f__ghz, OxygenData::f_0[i], delta_f__ghz, delta);

        N += S_i * F_i;
    }

    double N_D = NonresonantDebyeAttenuation(f__ghz, e__hPa, P__hPa, theta);

    return N + N_D;
}

// Equation 2b
double WaterVapourRefractivity(double f__ghz, double T__kelvin, double e__hPa, double P__hPa)
{
    double theta = 300 / T__kelvin;

    double N = 0;

    for (int i = 0; i < WaterVapourData::f_0.size(); i++)
    {
        double S_i = 0.1 * WaterVapourData::b_1[i] * e__hPa * pow(theta, 3.5) * exp(WaterVapourData::b_2[i] * (1 - theta));    // Equation 3

        // compute the width of the line
        double delta_f__ghz = 1e-4 * WaterVapourData::b_3[i] * (P__hPa * pow(theta, WaterVapourData::b_4[i]) + WaterVapourData::b_5[i] * e__hPa * pow(theta, WaterVapourData::b_6[i]));

        // modify the line width to account for Doppler broadening of water vapour lines
        double term1 = 0.217 * pow(delta_f__ghz, 2) + (2.1316e-12 * pow(WaterVapourData::f_0[i], 2) / theta);
        delta_f__ghz = 0.535 * delta_f__ghz + sqrt(term1);

        double delta = 0;
        double F_i = LineShapeFactor(f__ghz, WaterVapourData::f_0[i], delta_f__ghz, delta);

        N += S_i * F_i;
    }

    return N;
}

// Equation 5
double LineShapeFactor(double f__ghz, double f_i__ghz, double delta_f__ghz, double delta)
{
    double term1 = f__ghz / f_i__ghz;
    double term2 = (delta_f__ghz - delta * (f_i__ghz - f__ghz)) / (pow(f_i__ghz - f__ghz, 2) + pow(delta_f__ghz, 2));
    double term3 = (delta_f__ghz - delta * (f_i__ghz + f__ghz)) / (pow(f_i__ghz + f__ghz, 2) + pow(delta_f__ghz, 2));

    double F_i = term1 * (term2 + term3);

    return F_i;
}

double NonresonantDebyeAttenuation(double f__ghz, double e__hPa, double P__hPa, double theta)
{
    // width parameter for the Debye spectrum; Equation 9
    double d = 5.6e-4 * (P__hPa + e__hPa) * pow(theta, 0.8);

    // Equation 8
    double frac_1 = 6.14e-5 / (d * (1 + pow(f__ghz / d, 2)));
    double frac_2 = (1.4e-12 * P__hPa * pow(theta, 1.5)) / (1 + 1.9e-5 * pow(f__ghz, 1.5));
    double N_D = f__ghz * P__hPa * pow(theta, 2) * (frac_1 + frac_2);

    return N_D;
}
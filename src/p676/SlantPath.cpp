#include "../../include/p676.h"
#include "../../include/p835.h"

// Calculation the slant path attenuation due to atmospheric gases
int SlantPathAttenuation(double f__ghz, double h_1__km, double h_2__km, double beta_1__rad,
    SlantPathAttenuationResult* result)
{
    RayTraceConfig config;
    config.temperature = GlobalTemperature;
    config.dry_pressure = GlobalPressure;
    config.wet_pressure = GlobalWetPressure;

    if (beta_1__rad > PI / 2)
    {
        // negative elevation angle
        // find h_G and then trace in each direction
        // see Section 2.2.2

        // compute refractive index at h_1
        double p__hPa = config.dry_pressure(h_1__km);
        double T__kelvin = config.temperature(h_1__km);
        double e__hPa = config.wet_pressure(h_1__km);

        double n_1 = RefractiveIndex(p__hPa, T__kelvin, e__hPa);

        // set initial h_G at mid-point between h_1 and surface of the earth
        // then binary search to converge
        double h_G__km = h_1__km;
        double delta = h_1__km / 2;
        double diff = 100;

        double n_G;
        double grazing_term;
        double start_term;
        do
        {
            if (diff > 0)
                h_G__km -= delta;
            else
                h_G__km += delta;
            delta /= 2;

            p__hPa = config.dry_pressure(h_G__km);
            T__kelvin = config.temperature(h_G__km);
            e__hPa = config.wet_pressure(h_G__km);

            n_G = RefractiveIndex(p__hPa, T__kelvin, e__hPa);

            grazing_term = n_G * (a_0__km + h_G__km);
            start_term = n_1 * (a_0__km + h_1__km) * sin(beta_1__rad);

            diff = grazing_term - start_term;
        } while (abs(diff) > 0.001);

        // converged on h_G.  Now call RayTrace in both directions with grazing angle
        SlantPathAttenuationResult result_1, result_2;
        double beta_graze__rad = PI / 2;
        RayTrace(f__ghz, h_G__km, h_1__km, beta_graze__rad, config, &result_1);
        RayTrace(f__ghz, h_G__km, h_2__km, beta_graze__rad, config, &result_2);

        result->angle__rad = result_2.angle__rad;
        result->A_gas__db = result_1.A_gas__db + result_2.A_gas__db;
        result->a__km = result_1.a__km + result_2.a__km;
        result->bending__rad = result_1.bending__rad + result_2.bending__rad;
        result->delta_L__km = result_1.delta_L__km + result_2.delta_L__km;
    }
    else
    {
        RayTrace(f__ghz, h_1__km, h_2__km, beta_1__rad, config, result);
    }

    return 0;
}
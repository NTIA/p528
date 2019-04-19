#include <math.h>
#include "..\include\p528.h"

// References:
//  - [NBS 4]   CRPL Exponential Reference Atmosphere.  Bean and Thayer.  NBS Monograph 4.  1959.
//  - [Thayer]  A Rapid and Accurate Ray Tracing Algorithm for a Horizontally Stratified
//              Atmosphere.  Thayer.  1967.

/*=============================================================================
 |
 |  Description:  This function computes the ray tracing algorithm
 |                as described in Annex 2, Section 5 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  N_s           - Surface refractivity
 |                h_rx__km      - Height of the terminal, in km
 |
 |      Outputs:  arc_distance  - Arc distance, in km
 |                theta_rx      - Angle, in radian
 |
 *===========================================================================*/
void RayTrace(double N_s, double h_rx__km, double *arc_distance, double *theta_rx)
{
    if (h_rx__km == 0)                                      // both terminals are on the surface of the Earth
    {
        arc_distance = 0;
        theta_rx = 0;
        return;
    }

    // Reference atmosphere is 25 layers;
    int LAYERS = 25;

    // [NBS 4] 25-layer Reference Atmosphere
    double h_i__km[] = {
        0.000, 0.010, 0.020, 0.050,
        0.100, 0.200, 0.305, 0.500, 0.700,
        1.000, 1.524, 2.000, 3.048, 5.000, 7.000,
        10.00, 20.00, 30.48, 50.00, 70.00, 90.00,
        110.0, 225.0, 350.0, 475.0 };

    double r_ii__km, r_i__km;
    double theta_ii, theta_i;
    double N_ii;
    double n_ii, n_i;
    double A_i;

    double delta_N = -7.32 * exp(0.005577 * N_s);
    double C_e = log(N_s / (N_s + delta_N));

    // setting up tracing limit parameters
    double theta_takeoff = 0;						        // take off angle (grazing)
    double r_tx__km = a_0__km;                		        // radius of tx (surface)
    double r_rx__km = a_0__km + h_rx__km;            		// radius of rx
    double N_tx = N_s;                       		        // refractivity (N-Units) at tx (surface)
    double n_tx = 1.0 + (N_tx * 1.0e-6);   				    // refractive index at tx (surface)
    double N_rx = N_s * exp(-C_e * h_rx__km);		        // refractivity (N-Units) at rx
    double n_rx = 1.0 + (N_rx * 1.0e-6);	                // refractive index at rx
    double tau = 0.0;										// bending angle

    // initializing loop parameters
    theta_i = theta_takeoff;
    n_i = n_tx;
    r_i__km = r_tx__km;

    // trace the ray through the atmosphere
    for (int i = 0; i < LAYERS - 1; i++)
    {
        // compute the top of atmospheric layer
        r_ii__km = a_0__km + h_i__km[i + 1];		        // radius
        N_ii = N_s * exp(-C_e * h_i__km[i + 1]);            // refractivity (N-Units)
        n_ii = 1.0 + (N_ii * 1.0e-6);                       // refractive index

        // is the rx in the current layer?
        if (r_ii__km > r_rx__km)
        {
            // clamp the top of the layer to the height of rx
            r_ii__km = r_rx__km;
            n_ii = n_rx;
        }

        theta_ii = acos((r_i__km / r_ii__km) * (n_i / n_ii) * cos(theta_i));    // [Thayer, Equ 1], rearranged
        A_i = (log(n_ii) - log(n_i)) / (log(r_ii__km) - log(r_i__km));

        double tau_i = (theta_ii - theta_i) * -A_i / (A_i + 1.0);       // [Thayer, Equ 11]
        tau += tau_i;

        // top of current layer becomes bottom of next layer
        theta_i = theta_ii;
        n_i = n_ii;
        r_i__km = r_ii__km;

        // have we traced to rx yet?
        if (r_ii__km == r_rx__km)
            break;
    }

    // above the atmosphere - ray travels in a straight line relative to curve earth surface
    if (r_ii__km < r_rx__km)
        theta_ii = acos(cos(theta_i) * n_i * r_ii__km / r_rx__km);      // [Thayer, Equ 1], rearranged with n = 1

    double central_angle = (theta_ii - theta_takeoff + tau);            // [Thayer, Equ 2], rearranged
    *arc_distance = a_0__km * central_angle;
    *theta_rx = theta_ii;

    return;
}
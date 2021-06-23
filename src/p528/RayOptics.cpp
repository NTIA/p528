#include <math.h>
#include "..\..\include\p528.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This function computes the line-of-sight ray optics
 |                as described in Annex 2, Section 7 of
 |                Recommendation ITU-R P.528-5, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  terminal_1    - Structure holding low terminal parameters
 |                terminal_2    - Structure holding high terminal parameters
 |                psi           - Reflection angle, in radians
 |
 |      Outputs:  params        - Structure holding resulting parameters
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void RayOptics(Terminal *terminal_1, Terminal *terminal_2, double psi, LineOfSightParams *params)
{
    double z = (a_0__km / a_e__km) - 1;       // [Eqn 7-1]
    double k_a = 1 / (1 + z * cos(psi));      // [Eqn 7-2]
    params->a_a__km = a_0__km * k_a;          // [Eqn 7-3]

    double delta_h_a1__km = terminal_1->delta_h__km * (params->a_a__km - a_0__km) / (a_e__km - a_0__km);  // [Eqn 7-4]
    double delta_h_a2__km = terminal_2->delta_h__km * (params->a_a__km - a_0__km) / (a_e__km - a_0__km);  // [Eqn 7-4]

    double H__km[2] = { 0 };
    H__km[0] = terminal_1->h_r__km - delta_h_a1__km;    // [Eqn 7-5]
    H__km[1] = terminal_2->h_r__km - delta_h_a2__km;    // [Eqn 7-5]

    double Hprime__km[2] = { 0 };
    for (int i = 0; i < 2; i++)
    {
        params->z__km[i] = params->a_a__km + H__km[i];                                  // [Eqn 7-6]
        params->theta[i] = acos(params->a_a__km * cos(psi) / params->z__km[i]) - psi;   // [Eqn 7-7]
        params->D__km[i] = params->z__km[i] * sin(params->theta[i]);                    // [Eqn 7-8]

        // [Eqn 7-9]
        if (psi > 1.56)
            Hprime__km[i] = H__km[i];
        else
            Hprime__km[i] = params->D__km[i] * tan(psi);
    }

    double delta_z = abs(params->z__km[0] - params->z__km[1]);   // [Eqn 7-10]

    params->d__km = MAX(params->a_a__km * (params->theta[0] + params->theta[1]), 0);  // [Eqn 7-11]

    double alpha = atan((Hprime__km[1] - Hprime__km[0]) / (params->D__km[0] + params->D__km[1]));  // [Eqn 7-12]
    params->r_0__km = MAX(delta_z, (params->D__km[0] + params->D__km[1]) / cos(alpha));            // [Eqn 7-13]
    params->r_12__km = (params->D__km[0] + params->D__km[1]) / cos(psi);                           // [Eqn 7-14]

    params->delta_r = 4.0 * Hprime__km[0] * Hprime__km[1] / (params->r_0__km + params->r_12__km);  // [Eqn 7-15]

    params->theta_h1__rad = alpha - params->theta[0];                // [Eqn 7-16]
    params->theta_h2__rad = -(alpha + params->theta[1]);             // [Eqn 7-17]
}
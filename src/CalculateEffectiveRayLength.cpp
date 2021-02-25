#include <math.h>
#include "..\include\p528.h"

/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This function computes the effective ray length as described
 |                in Annex 2, Section 12 of Recommendation ITU-R P.528-4,
 |                "Propagation curves for aeronautical mobile and
 |                radionavigation services using the VHF, UHF and SHF bands"
 |
 |        Input:  z_1__km   - The radial of the low terminal, in km
 |                z_2__km   - The radial of the high terminal, in km
 |                a__km     - The radius of the Earth to be used, in km
 |                d_arc__km - The arc distance, in km
 |                beta__rad - The takeoff angle, in radians
 |                T_e__km   - The thickness of the absorbing layer, in km
 |
 |       Returns: r_e__km   - The effective ray length, in km.
 |
 *===========================================================================*/
double CalculateEffectiveRayLength(double z_1__km, double z_2__km, double a__km, double d_arc__km, double beta__rad, double T_e__km)
{
    double alpha__rad = (PI / 2.0) + beta__rad;                             // [Eqn 146]
    double z_T__km = a__km + T_e__km;                                       // [Eqn 147]

    double r_e__km;

    if (z_2__km <= z_T__km)
        r_e__km = d_arc__km;                                                // [Eqn 148]
    else if (z_T__km < z_1__km)
    {
        if (beta__rad > 0)
            r_e__km = 0;
        else
        {
            double z_c__km = z_1__km * sin(alpha__rad);                     // [Eqn 149]

            // [Eqn 150]
            if (z_T__km <= z_c__km)
                r_e__km = 0.0;
            else
                r_e__km = 2.0 * z_T__km * sin(acos(z_c__km / z_T__km));
        }
    }
    else
    {
        double A_q__rad = asin(z_1__km * sin(alpha__rad) / z_T__km);        // [Eqn 151]
        double A_e__rad = PI - (alpha__rad + A_q__rad);                     // [Eqn 152]

        // [Eqn 153]
        if (A_e__rad == 0.0)
            r_e__km = z_T__km - z_1__km;
        else
            r_e__km = (z_1__km * sin(A_e__rad)) / sin(A_q__rad);
    }

    return r_e__km;
}
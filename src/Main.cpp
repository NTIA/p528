#include <math.h>
#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  This is the main entry point to this software.
 |                It describes Annex 2, Section 3 of Recommendation ITU-R
 |                P.528-4, "Propagation curves for aeronautical mobile and
 |                radionavigation services using the VHF, UHF and SHF bands"
 |
 |        Input:  d__km             - Path distance, in km
 |                h_1__meter        - Height of the low terminal, in meters
 |                h_2__meter        - Height of the high terminal, in meters
 |                f__mhz            - Frequency, in MHz
 |                time_percentage   - Time percentage
 |
 |      Outputs:  A__db             - Total path loss, in dB
 |                A_fs__db          - Free space path loss, in dB
 |                d__km             - Path distance used, in km
 |                mode              - Mode of propagation:
 |                                      + 1 : Line-of-Sight
 |                                      + 2 : Diffraction
 |                                      + 3 : Troposcatter
 |
 *===========================================================================*/
int Main(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, Result* result)
{
    Terminal terminal_1;
    Terminal terminal_2;
    TroposcatterParams tropo;
    Path path;
    LineOfSightParams los_params;

    return MainEx(d__km, h_1__meter, h_2__meter, f__mhz, time_percentage, result,
        &terminal_1, &terminal_2, &tropo, &path, &los_params);
}
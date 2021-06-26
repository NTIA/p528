#include <math.h>
#include "../../include/p528.h"
#include "../../include/p676.h"

/*=============================================================================
 |
 |  Description:  This file computes the terminal geometry as described
 |                in Annex 2, Section 4 of Recommendation ITU-R P.528-5,
 |                "Propagation curves for aeronautical mobile and
 |                radionavigation services using the VHF, UHF and SHF bands"
 |
 |        Input:  f__mhz    - Frequency, in MHz
 |
 |      Outputs:  terminal  - Structure containing parameters dealing
 |                            with the geometry of the terminal
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void TerminalGeometry(double f__mhz, Terminal *terminal)
{
    double theta_tx__rad = 0;
    SlantPathAttenuationResult result;
    SlantPathAttenuation(f__mhz / 1000, 0, terminal->h_r__km, PI / 2 - theta_tx__rad, &result);
    terminal->theta__rad = PI / 2 - result.angle__rad;
    terminal->A_a__db = result.A_gas__db;
    terminal->a__km = result.a__km;

    // compute arc distance
    double central_angle = ((PI / 2 - result.angle__rad) - theta_tx__rad + result.bending__rad);            // [Thayer, Equ 2], rearranged
    terminal->d_r__km = a_0__km * central_angle;

    terminal->phi__rad = terminal->d_r__km / a_e__km;                   // [Eqn 4-1]
    terminal->h_e__km = (a_e__km / cos(terminal->phi__rad)) - a_e__km;  // [Eqn 4-2]

    terminal->delta_h__km = terminal->h_r__km - terminal->h_e__km;      // [Eqn 4-3]
}

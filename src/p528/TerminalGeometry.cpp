/** @file TerminalGeomentry.cpp
 * Computes the terminal geometry 
 */
#include <math.h>
#include "p528.h"
#include "p676.h"

/*******************************************************************************
 * Computes the terminal geometry as described
 *
 * References:
 *     - Recommendation ITU‑R P.528‑5 "Propagation curves for aeronautical mobile
 * and radionavigation services using the VHF, UHF and SHF bands", Annex 2, 
 * Section 4.
 *
 * @param[in] f__mhz    Frequency, in MHz
 * @param[in] terminal  Structure containing parameters dealing with the geometry 
 *                      of the terminal
 * @par Returns
 *      Nothing.
 ******************************************************************************/
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

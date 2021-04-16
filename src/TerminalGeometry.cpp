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
 |  Description:  This file computes the terminal geometry as described
 |                in Annex 2, Section 4 of Recommendation ITU-R P.528-4,
 |                "Propagation curves for aeronautical mobile and
 |                radionavigation services using the VHF, UHF and SHF bands"
 |
 |        Input:  a_e__km   - Effective earth radius, in km
 |
 |      Outputs:  terminal  - Structure containing parameters dealing
 |                            with the geometry of the terminal
 |
 *===========================================================================*/
void TerminalGeometry(double f__mhz, Terminal *terminal)
{
	// Step 1
	RayTrace(f__mhz, 0, terminal->h_r__km, 0, &terminal->d_r__km, &terminal->theta__rad, &terminal->A_a__db, &terminal->a__km);

	// Step 2
	terminal->phi__rad = terminal->d_r__km / a_e__km;       // [Eqn 24]

	// [Eqn 25]
	if (terminal->phi__rad <= 0.1)  // conditional is using the small angle approximation for cosine
		terminal->h_e__km = pow(terminal->d_r__km, 2) / (2.0 * a_e__km);
	else
		terminal->h_e__km = (a_e__km / cos(terminal->phi__rad)) - a_e__km;

	// Step 3
	if (terminal->h_e__km <= terminal->h_r__km)
	{
		terminal->h__km = terminal->h_e__km;
		terminal->d__km = terminal->d_r__km;
	}
	else
	{
		terminal->h__km = terminal->h_r__km;
		terminal->d__km = sqrt(2.0 * a_e__km * terminal->h_r__km); // approx. of horizon distance for low terminal heights
	}

	// Step 4
	terminal->delta_h__km = terminal->h_r__km - terminal->h__km;    // [Eqn 29]

	// Step 5, Really if h_e > h_r => h_e == h_r from above
	if (terminal->delta_h__km <= 0.0)
	{
		terminal->theta__rad = sqrt(2.0 * terminal->h_r__km / a_e__km);
		terminal->d__km = sqrt(2.0 * terminal->h_r__km * a_e__km);
	}
}

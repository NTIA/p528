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
int P528(double d__km, double h_1__meter, double h_2__meter, double f__mhz, int T_pol, double time_percentage, Result *result)
{
	Terminal terminal_1;
	Terminal terminal_2;
	TroposcatterParams tropo;
	Path path;
	LineOfSightParams los_params;

	// reset Results struct
	result->A_fs__db = 0;
	result->A__db = 0;
	result->d__km = 0;
	result->los_iterations = 0;
	result->propagation_mode = PROP_MODE__NOT_SET;

	int err = ValidateInputs(d__km, h_1__meter, h_2__meter, f__mhz, T_pol, time_percentage);
    if (err != SUCCESS)
    {
        if (err == ERROR_HEIGHT_AND_DISTANCE)
        {
            result->A_fs__db = 0;
            result->A__db = 0;
            result->d__km = 0;
            return SUCCESS;
        }
        else
            return err;
    }

	double N_s = N_0;
	path.a_e__km = a_0__km / (1.0 - 0.04665 * exp(0.005577 * N_s));

	/////////////////////////////////////////////
	// Compute terminal geometries
	//

	// Step 1 for low terminal
	terminal_1.h_r__km = h_1__meter / 1000;
	TerminalGeometry(f__mhz, N_s, path.a_e__km, &terminal_1);

	// Step 1 for high terminal
	terminal_2.h_r__km = h_2__meter / 1000;
	TerminalGeometry(f__mhz, N_s, path.a_e__km, &terminal_2);

	//
	// Compute terminal geometries
	/////////////////////////////////////////////

	// Step 2
	path.d_ML__km = terminal_1.d__km + terminal_2.d__km;                                // [Eqn 4]

	/////////////////////////////////////////////
	// Smooth earth diffraction line calculations
	//

	// Step 3.1
	double d_3__km = path.d_ML__km + 0.5 * pow(pow(path.a_e__km, 2) / f__mhz, THIRD);   // [Eqn 5]
	double d_4__km = path.d_ML__km + 1.5 * pow(pow(path.a_e__km, 2) / f__mhz, THIRD);   // [Eqn 6]

	// Step 3.2
	double A_3__db = SmoothEarthDiffraction(terminal_1.d__km, terminal_2.d__km, f__mhz, d_3__km, T_pol);
	double A_4__db = SmoothEarthDiffraction(terminal_1.d__km, terminal_2.d__km, f__mhz, d_4__km, T_pol);

	// Step 3.3
	double M_d = (A_4__db - A_3__db) / (d_4__km - d_3__km);                             // [Eqn 7]
	double A_d0 = A_4__db - M_d * d_4__km;                                              // [Eqn 8]

	// Step 3.4
	double A_dML__db = (M_d * path.d_ML__km) + A_d0;                                    // [Eqn 9]
	path.d_d__km = -(A_d0 / M_d);                                                       // [Eqn 10]

	//
	// End smooth earth diffraction line calculations
	/////////////////////////////////////////////////

	double K_LOS = 0;

	// Step 4.  If the path is in the Line-of-Sight range, call LOS and then exit
	if (path.d_ML__km - d__km > 0.001)
	{
		result->propagation_mode = PROP_MODE__LOS;
		LineOfSight(&path, terminal_1, terminal_2, &los_params, f__mhz, -A_dML__db, time_percentage, d__km, T_pol, result, &K_LOS);

		return SUCCESS;
	}
	else
	{
		// get K_LOS
		LineOfSight(&path, terminal_1, terminal_2, &los_params, f__mhz, -A_dML__db, time_percentage, path.d_ML__km - 1, T_pol, result, &K_LOS);

		// Step 6.  Search past horizon to find crossover point between Diffraction and Troposcatter models
		int CASE;
		double d_crx__km;
		int rtn = TranshorizonSearch(&path, terminal_1, terminal_2, f__mhz, N_s, A_dML__db, &M_d, &A_d0, &d_crx__km, &CASE);

		/////////////////////////////////////////////
		// Compute terrain attenuation, A_T__db
		//

		// Step 7.1
		double A_d__db = M_d * d__km + A_d0;                    // [Eqn 17]

		// Step 7.2
		Troposcatter(path, terminal_1, terminal_2, d__km, f__mhz, N_s, &tropo);

		// Step 7.3
		double A_T__db;
		if (d__km < d_crx__km)
		{
			A_T__db = -A_d__db;
			result->propagation_mode = PROP_MODE__DIFFRACTION;
		}
		else
		{
			if (CASE == 1)
			{
				if (tropo.A_s__db <= A_d__db)
				{
					A_T__db = -tropo.A_s__db;
					result->propagation_mode = PROP_MODE__SCATTERING;
				}
				else
				{
					A_T__db = -A_d__db;
					result->propagation_mode = PROP_MODE__DIFFRACTION;
				}
			}
			else
			{
				A_T__db = -tropo.A_s__db;
				result->propagation_mode = PROP_MODE__SCATTERING;
			}
		}

		//
		// Compute terrain attenuation, A_T__db
		/////////////////////////////////////////////

		/////////////////////////////////////////////
		// Compute free-space loss
		//

		// Step 8
		double r_1__km = sqrt(pow(terminal_1.h_r__km, 2) + 4.0 * (a_0__km + terminal_1.h_r__km) * (a_0__km)* pow(sin(0.5 * terminal_1.d__km / a_0__km), 2));    // [Eqn 20]
		double r_2__km = sqrt(pow(terminal_2.h_r__km, 2) + 4.0 * (a_0__km + terminal_2.h_r__km) * (a_0__km)* pow(sin(0.5 * terminal_2.d__km / a_0__km), 2));    // [Eqn 20]
		double r_fs__km = r_1__km + r_2__km + tropo.d_s__km;                            // [Eqn 21]

		result->A_fs__db = -32.45 - 20.0 * log10(f__mhz) - 20.0 * log10(r_fs__km);      // [Eqn 22]

		//
		// Compute free-space loss
		/////////////////////////////////////////////

		/////////////////////////////////////////////
		// Compute variability
		//

		double f_theta_h = 1;

		double Y_e__db, Y_e_50__db, A_Y;
		LongTermVariability(terminal_1.h_r__km, terminal_2.h_r__km, d__km, f__mhz, time_percentage, f_theta_h, A_T__db, &Y_e__db, &A_Y);
		LongTermVariability(terminal_1.h_r__km, terminal_2.h_r__km, d__km, f__mhz, 0.50, f_theta_h, A_T__db, &Y_e_50__db, &A_Y);

		double ANGLE = 0.02617993878;
		double K_t__db;
		if (tropo.theta_s >= ANGLE) // theta_s > 1.5 deg
			K_t__db = 20;
		else if (tropo.theta_s <= 0.0)
			K_t__db = K_LOS;
		else
			K_t__db = (tropo.theta_s * (20.0 - K_LOS) / ANGLE) + K_LOS;

		double Y_pi_50__db = 0.0;   //  zero mean
		double Y_pi__db = NakagamiRice(K_t__db, time_percentage);

		double Y_total__db = CombineDistributions(Y_e_50__db, Y_e__db, Y_pi_50__db, Y_pi__db, time_percentage);

		//
		// Compute variability
		/////////////////////////////////////////////

		/////////////////////////////////////////////
		// Atmospheric absorption for transhorizon path
		//

		double d_arc__km, theta_rx__rad, A_a_hv__db;
		RayTrace(f__mhz, tropo.h_v__km, 0, &d_arc__km, &theta_rx__rad, &A_a_hv__db);

		double A_a__db = -(terminal_1.A_a__db + terminal_2.A_a__db + 2 * A_a_hv__db);

		//
		// Atmospheric absorption for transhorizon path
		/////////////////////////////////////////////

		result->d__km = d__km;
		result->A__db = result->A_fs__db + A_a__db + A_T__db + Y_total__db;     // [Eqn 23]

		return rtn;
	}
}
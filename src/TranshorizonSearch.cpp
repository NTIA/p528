#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  This file computes Step 6 in Annex 2, Section 3 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  path              - Structure containing parameters dealing
 |                                    with the propagation path
 |                terminal_1        - Structure containing parameters dealing
 |                                    with the geometry of the low terminal
 |                terminal_2        - Structure containing parameters dealing
 |                                    with the geometry of the high terminal
 |                f__mhz            - Frequency, in MHz
 |                N_s               - Surface refractivity, in N-Units
 |                A_dML__db         - Diffraction loss at d_ML, in dB
 |
 |      Outputs:  M_d               - Slope of the diffraction line
 |                A_d0              - Intercept of the diffraction line
 |                d_crx__km         - Final search distance, in km
 |                CASE              - Case as defined in Step 6.5
 |
 *===========================================================================*/
void TranshorizonSearch(Path* path, Terminal terminal_1, Terminal terminal_2, double f__mhz,
    double N_s, double A_dML__db, double *M_d, double *A_d0, double* d_crx__km, int *CASE)
{
    *CASE = CONST_MODE__SEARCH;
    int k = 0;

    TroposcatterParams tropo_params;
    tropo_params.A_s__db = 0;

    // Step 6.1.  Initialize search parameters
    double d_search__km[2];
    d_search__km[0] = path->d_ML__km + 3;
    d_search__km[1] = d_search__km[0] - 1;

    double A_s__db[2] = { 0 };
    double M_s = 0;

    int SEARCH_LIMIT = 100; // 100 km beyond starting point

    for (int i_search = 0; i_search < SEARCH_LIMIT; i_search++)
    {
        A_s__db[1] = A_s__db[0];

        // Step 6.2
        Troposcatter(*path, terminal_1, terminal_2, d_search__km[0], f__mhz, N_s, &tropo_params);
        A_s__db[0] = tropo_params.A_s__db;

        // if loss is less than 20 dB, the result is not within valid part of model
        if (tropo_params.A_s__db < 20.0)
        {
            d_search__km[1] = d_search__km[0];
            d_search__km[0]++;
            continue;
        }

        k++;
        if (k <= 1) // need two points to draw a line and we don't have them both yet
        {
            d_search__km[1] = d_search__km[0];
            d_search__km[0]++;
            continue;
        }

        // Step 6.3
        M_s = (A_s__db[0] - A_s__db[1]) / (d_search__km[0] - d_search__km[1]);          // [Eqn 10]

        // Verify the tropo line is sloping to increase loss with distance (inverted sign)
        if (M_s <= -0.01)
        {
            k = 0;
            d_search__km[1] = d_search__km[0];
            d_search__km[0]++;
            continue;
        }

        if (M_s <= *M_d)
        {
            *d_crx__km = d_search__km[0];

            double A_d__db = *A_d0 + (*M_d * d_search__km[1]);

            // Step 6.5
            if (A_s__db[1] >= A_d__db)
                *CASE = 1;
            else
            {
                // Adjust the diffraction line to the troposcatter model
                *M_d = (A_s__db[1] - A_dML__db) / (d_search__km[1] - path->d_ML__km);   // [Eqn 11]
                *A_d0 = A_s__db[1] - (d_search__km[1] * *M_d);                          // [Eqn 12]

                *CASE = 2;
            }

            break;
        }

        d_search__km[1] = d_search__km[0];
        d_search__km[0]++;
    }

    if (*CASE == CONST_MODE__SEARCH)
    {
        *CASE = CONST_MODE__DIFFRACTION;
        *d_crx__km = d_search__km[1];
    }
}
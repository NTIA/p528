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
 |  Description:  This function computes the atmospheric absorption
 |                parameters as described in Annex 2, Section 14 of
 |                Recommendation ITU-R P.528-4, "Propagation curves for
 |                aeronautical mobile and radionavigation services using
 |                the VHF, UHF and SHF bands"
 |
 |        Input:  f__mhz    - The frequency, in MHz
 |
 |      Outputs:  gamma_oo  - Oxygen absorption rate, in dB/km
 |                gamma_ow  - Water vapor absorption rate, in dB/km
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void AtmosphericAbsorptionParameters(double f__mhz, double *gamma_oo, double *gamma_ow)
{
    // Frequencies, in MHz
    double freqs__mhz[20] = { 100, 150, 205, 300, 325, 350, 400, 550, 700, 1000, 
        1520, 2000, 3000, 3400, 4000, 4900, 8300, 10200, 15000, 17000 };

    // Oxygen absorption rates, in dB/km.  Corresponds to frequency array
    double oxygen_data[20] = { 0.00019, 0.00042, 0.00070, 0.00096, 0.0013, 0.0015, 0.0018, 
        0.0024, 0.003, 0.0042, 0.005, 0.007, 0.0088, 0.0092, 0.010, 0.011, 0.014, 0.015, 
        0.017, 0.018 };

    // Water vapor absorption rates, in dB/km.  Corresponds to frequency array
    double water_data[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
        0.0, 0.0001, 0.00017, 0.00034, 0.0021, 0.009, 0.025, 0.045 };

    for (int i = 0; i < 20; i++)
    {
        if (f__mhz == freqs__mhz[i])
        {
            *gamma_oo = oxygen_data[i];
            *gamma_ow = water_data[i];

            return;
        }

        if (f__mhz < freqs__mhz[i])
        {
            // Scale factor for interpolation
            double S = (log10(f__mhz) - log10(freqs__mhz[i - 1])) / (log10(freqs__mhz[i]) - log10(freqs__mhz[i - 1]));

            double X = (S*(log10(oxygen_data[i]) - log10(oxygen_data[i - 1]))) + log10(oxygen_data[i - 1]);
            *gamma_oo = pow(10.0, X);

            if (i < 14)
                *gamma_ow = 0.0000;
            else
            {
                double Y = (S*(log10(water_data[i]) - log10(water_data[i - 1]))) + log10(water_data[i - 1]);
                *gamma_ow = pow(10.0, Y);
            }

            return;
        }
    }
}

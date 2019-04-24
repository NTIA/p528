# NOTICE!
**Please note that this software is based on a proposed update to Rec P.528 which will be considered during the May 2019 Study Group 3 meetings.  The current in-force Recommendation is P.525-3 (not P.528-4).  This code could undergo changes, including but not limited to breaking and functional changes, up until the conclusion of the Study Group 3 meetings, based on the outcomes of the meetings.**

---

# US Reference Implementation of Rec ITU-R P.528-4 #

This repo contains the US Reference Software Implementation of Recommendation [ITU-R Rec P.528](http://www.itu.int/rec/R-REC-P.528/en): Propagation curves for aeronautical mobile and radionavigation services using the VHF, UHF and SHF bands.

# Summary of Software #

This repo contains C++ code that implements Annex 2 of Rec P.528, the Step-by-Step method to computing propagation loss for air-to-ground paths.  It was developed to provide maximum traceability back to the Recommendation text.  Wherever possible, equation numbers where provided.  It is assumed that a user reviewing this source code would have a copy of the Recommendation's text available as a primary reference.

The software is designed to be built into a DLL (or corresponding library for non-Windows systems).  A Visual Studio project file is provided for Windows users to support the build process and configuration.

# Inputs #

 * __double__ `d__km` : Path distance between terminals, in km.  0 <= `d__km`
 * __double__ `h_1__meter` : Height of the low terminal, in meters. 1.5 <= `h_1__meter`
 * __double__ `h_2__meter` : Height of the high terminal, in meters.  1.5 <= `h_2__meter`
 * __double__ `f__mhz` : Frequency, in MHz.  125 <= `f__mhz` <= 15 500
 * __double__ `time_percentage` : Time percentage.  0.01 <= `time_percentage` <= 0.99
 
# Outputs #

Outputs to P.528 are contained within a defined `Results` structure.

 * __double__ `d__km` : Path distance, in km.  Could be slightly different than specified in input variable if within LOS region
 * __double__ `A__db` : Total path loss, in dB
 * __double__ `A_fs__db` : Free space path loss, in dB
 * __double__ `propagation_mode`
   * 1 = Line of Sight
   * 2 = Diffraction
   * 3 = Troposcatter

# Return Codes #

The following is a list of possible return codes, including the corresponding defined constant name as defined in `p528.h`:

| Value | Const Name                       | Description  |
| ------|----------------------------------|-------------|
|     0 | `SUCCESS`                        | Successful execution |
|     1 | `ERROR_VALIDATION__D_KM`         | Path distance must be >= 0 km |
|     2 | `ERROR_VALIDATION__H_1`          | Low terminal height must be >= 1.5 meters |
|     3 | `ERROR_VALIDATION__H_2`          | High terminal height must be >= 1.5 meters |
|     4 | `ERROR_VALIDATION__TERM_GEO`     | Low terminal must be <= high terminal height |
|     5 | `ERROR_VALIDATION__F_MHZ_LOW`    | Frequency must be >= 125 MHz |
|     6 | `ERROR_VALIDATION__F_MHZ_HIGH`   | Frequency must be <= 15 500 MHz |
|     7 | `ERROR_VALIDATION__PERCENT_LOW`  | Time percentage must be >= 0.01 |
|     8 | `ERROR_VALIDATION__PERCENT_HIGH` | Time percentage must be <= 0.99 |
|    10 | `ERROR_HEIGHT_AND_DISTANCE`      | Terminals are occupying the same point in space (they are the same height and 0 km apart) |
|    20	| `WARNING__DFRAC_TROPO_REGION`    |	Warning that the diffraction and troposcatter model may not be physically consistent with each other. Caution should be taken when using the results |

# Example Values

The below table includes a select set of example inputs and outputs for testing purposes.  An extensive set of validation example values can be found [by downloading the P.528 CSV data files](https://www.itu.int/rec/R-REC-P.528/en).

| `d__km` | `h_1__meter` | `h_2__meter` | `f__mhz` | `time_percentage` | `A__db` |
| --------|--------------|--------------|----------|-------------------|---------|
|      15 |           10 |        1 000 |      500 |               0.5 |  -110.0 |
|     100 |          100 |       15 000 |    3 600 |               0.9 |  -151.2 |
|   1 500 |           15 |       10 000 |    5 700 |               0.1 |  -299.5 |

# Dependencies #

The P.528 source code is only dependent on the standard C++ build libraries, such as `math.lib`.

# Notes on Code Style #

 * In general, variables follow the naming convention in which a single underscore denotes a subscript (pseudo-LaTeX format), where a double underscore is followed by the units, i.e. h_1__meter.
 * Variables are named to match their corresponding mathematical variables in the underlying Recommendation text.

# Configuring and Building #

This project was developed and built using Microsoft Visual Studio 2017.  However, the source code is such that it takes no direct dependence upon the Windows operating system and can be built for any OS that supports the standard C++ libraries.

# Contact #

For questions, contact Billy Kozma, (303) 497-6082, wkozma@ntia.gov
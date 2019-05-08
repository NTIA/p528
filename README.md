# NOTICE!

**This software is based on a proposed update to ITU Recommendation P.528 which will be considered during the May 2019 ITU-R Study Group 3 meetings.  The current in-force Recommendation is P.525-3 (not P.528-4).  This code could undergo changes, including but not limited to breaking and functional changes, up until the conclusion of the Study Group 3 meetings, based on the outcomes of the meetings.**

# ITU-R Recommendation P.528-4 - U.S. Reference Implementation  #

This code repository contains the U.S. Reference Software Implementation of ITU-R Recommendation P.528. This Recommendation contains a method for predicting basic transmission loss in the frequency range 125-15 500 MHz for aeronautical and satellite services. Companion software ([p528-gui](https://github.com/NTIA/p528-gui)) provides a Graphical User Interface (GUI) that can be used with this software implementation.

This software is written in C++.  It implements Annex 2 of Rec P.528, the Step-by-Step method to computing propagation loss for air-to-ground paths.  

## Inputs ##

| Variable | Type       | Units  | Limits       | Description  |
|----------|------------|--------|--------------|--------------|
| `d__km`  | __double__ | km     | 0 <= `d__km` | Path distance between terminals |
| `h_1__meter` | __double__ | meter | 1.5 <= `h_1__meter` | Height of the low terminal |
| `h_2__meter` | __double__ | meter | 1.5 <= `h_2__meter` | Height of the high terminal |
| `f__mhz` | __double__ | MHz    | 125 <= `f__mhz` <= 15 500 | Frequency |
| `time_percentage` | __double__ | _N/A_ | 0.01 <= `time_percentage` <= 0.99 | Time percentage |
 
## Outputs ##

Outputs to P.528 are contained within a defined `Results` structure.

| Variable | Type | Units | Description |
|----------|---|---|---|
| `d__km`  | __double__ | km | Path distance.  Could be slightly different than specified in input variable if within LOS region |
| `A__db`  | __double__ | dB | Total path loss |
| `A_fs__db` | __double__ | dB | Free space path loss |
| `propagation_mode` | __int__ | _N/A_ | Mode of propagation. <ul><li>1 = Line of Sight</li><li>2 = Diffraction</li><li>3 = Troposcatter</li></ul> |

## Return Codes ##

Possible return codes, including the corresponding defined constant name as defined in `p528.h`:

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

## Example Values ##

The below table includes a select set of example inputs and outputs for testing purposes. The [P.528 CSV data files](https://www.itu.int/rec/R-REC-P.528/en) contain an extensive set of validation example values.

| `d__km` | `h_1__meter` | `h_2__meter` | `f__mhz` | `time_percentage` | `A__db` |
| --------|--------------|--------------|----------|-------------------|---------|
|      15 |           10 |        1 000 |      500 |               0.5 |  -110.0 |
|     100 |          100 |       15 000 |    3 600 |               0.9 |  -151.2 |
|   1 500 |           15 |       10 000 |    5 700 |               0.1 |  -299.5 |

## Notes on Code Style ##

 * In general, variables follow the naming convention in which a single underscore denotes a subscript (pseudo-LaTeX format), where a double underscore is followed by the units, i.e. h_1__meter.
 * Variables are named to match their corresponding mathematical variables in the underlying Recommendation text.
 * Wherever possible, equation numbers are provided.  It is assumed that a user reviewing this source code would have a copy of the Recommendation's text available as a primary reference.

## Configure and Build ##

The software is designed to be built into a DLL (or corresponding library for non-Windows systems).  The source code can be built for any OS that supports the standard C++ libraries.  A Visual Studio 2017 project file is provided for Windows users to support the build process and configuration.

## References ##

 * [ITU-R Recommendation P.528](https://www.itu.int/rec/R-REC-P.528/en)
 * [Rec P.528-4 Curve Visualizer Tool](https://github.com/NTIA/p528-gui)

# Contact #

For questions, contact Billy Kozma, (303) 497-6082, wkozma@ntia.gov

# Return Codes and Warning Flags #

## Return Codes ##

P.528 supports the following list of return codes.

| Value | Const Name                       | Description  |
| ------|----------------------------------|--------------|
|     0 | `SUCCESS`                        | Successful execution |
|     1 | `ERROR_VALIDATION__D_KM`         | Path distance must be >= 0 km |
|     2 | `ERROR_VALIDATION__H_1`          | Low terminal height must be >= 1.5 meters |
|     3 | `ERROR_VALIDATION__H_2`          | High terminal height must be >= 1.5 meters |
|     4 | `ERROR_VALIDATION__TERM_GEO`     | Low terminal must be <= high terminal height |
|     5 | `ERROR_VALIDATION__F_MHZ_LOW`    | Frequency must be >= 100 MHz |
|     6 | `ERROR_VALIDATION__F_MHZ_HIGH`   | Frequency must be <= 30 000 MHz |
|     7 | `ERROR_VALIDATION__PERCENT_LOW`  | Time percentage must be >= 1 |
|     8 | `ERROR_VALIDATION__PERCENT_HIGH` | Time percentage must be <= 99 |
|    10 | `ERROR_HEIGHT_AND_DISTANCE`      | Terminals are occupying the same point in space (they are the same height and 0 km apart) |


## Warning Flags ##

In addition to return codes, P.528 supports return warning flags to the caller.  Warning flags are bitwise set, allowing the P.528 model to return multiple warnings for a single function call.  Warning flags only need to be parsed if a return code of `SUCCESS_WITH_WARNINGS` is returned by a function call.

| Value  | Const Name                       | Description  |
| -------|----------------------------------|--------------|
| 0x00   | `NO_WARNINGS`                    | No warning flags |
| 0x01   | `WARNING__DFRAC_TROPO_REGION`    | Warning that the diffraction and troposcatter model may not be physically consistent with each other. Caution should be taken when using the result |
| 0x02   | `WARNING__HEIGHT_LIMIT_H_1`      | Terminal 1 height above limit defined within in-force Recommendation text.  Results should not be intepreted as an official prediction of Recommendation ITU-R P.528, but instead treated as informative |
| 0x04   | `WARNING__HEIGHT_LIMIT_H_2`      | Terminal 2 height above limit defined within in-force Recommendation text.  Results should not be intepreted as an official prediction of Recommendation ITU-R P.528, but instead treated as informative |
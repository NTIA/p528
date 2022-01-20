#include "../../include/p528.h"

/*=============================================================================
 |
 |  Description:  Validate the model input values
 |
 |        Input:  d__km             - Path distance, in km
 |                h_1__meter        - Height of the low terminal, in meters
 |                h_2__meter        - Height of the high terminal, in meters
 |                f__mhz            - Frequency, in MHz
 |                T_pol             - Code indicating either polarization
 |                                      + 0 : POLARIZATION__HORIZONTAL
 |                                      + 1 : POLARIZATION__VERTICAL
 |                p	                - Time percentage
 |
 |       Output:  warnings          - Warning codes
 |
 |      Returns:  SUCCESS, or validation error code
 |
 *===========================================================================*/
int ValidateInputs(double d__km, double h_1__meter, double h_2__meter, 
    double f__mhz, int T_pol, double p, int* warnings)
{
    int rtn = SUCCESS;

    if (d__km < 0)
        return ERROR_VALIDATION__D_KM;

    if (h_1__meter < 1.5 || h_1__meter > 80000)
        return ERROR_VALIDATION__H_1;

    if (h_2__meter < 1.5 || h_2__meter > 80000)
        return ERROR_VALIDATION__H_2;

    if (h_1__meter > 20000 && h_1__meter <= 80000)
        *warnings |= WARNING__HEIGHT_LIMIT_H_1;

    if (h_2__meter > 20000 && h_2__meter <= 80000)
        *warnings |= WARNING__HEIGHT_LIMIT_H_2;

    if (h_1__meter > h_2__meter)
        return ERROR_VALIDATION__TERM_GEO;

    if (f__mhz < 100)
        return ERROR_VALIDATION__F_MHZ_LOW;

    if (f__mhz > 30000)
        return ERROR_VALIDATION__F_MHZ_HIGH;

    if (T_pol != POLARIZATION__HORIZONTAL &&
        T_pol != POLARIZATION__VERTICAL)
        return ERROR_VALIDATION__POLARIZATION;

    if (p < 1)
        return ERROR_VALIDATION__PERCENT_LOW;

    if (p > 99)
        return ERROR_VALIDATION__PERCENT_HIGH;

    if (h_1__meter == h_2__meter && d__km == 0)
        return ERROR_HEIGHT_AND_DISTANCE;

    return SUCCESS;
}
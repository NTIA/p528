#include "..\include\p528.h"

int ValidateInputs(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage)
{
    if (d__km < 0)
        return ERROR_VALIDATION__D_KM;

    if (h_1__meter < 1.5)
        return ERROR_VALIDATION__H_1;

    if (h_2__meter < 1.5)
        return ERROR_VALIDATION__H_2;

    if (h_1__meter > h_2__meter)
        return ERROR_VALIDATION__TERM_GEO;

    if (f__mhz < 125)
        return ERROR_VALIDATION__F_MHZ_LOW;

    if (f__mhz > 15500)
        return ERROR_VALIDATION__F_MHZ_HIGH;

    if (time_percentage < 0.01)
        return ERROR_VALIDATION__PERCENT_LOW;

    if (time_percentage > 0.99)
        return ERROR_VALIDATION__PERCENT_HIGH;

    if (h_1__meter == h_2__meter && d__km == 0)
        return ERROR_HEIGHT_AND_DISTANCE;

    return SUCCESS;
}
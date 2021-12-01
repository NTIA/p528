#include "../../include/p676.h"

/*=============================================================================
 |
 |  Description:  Line-shape factor.  See Equation (5).
 |
 |        Input:  f__ghz        - Frequency, in GHz
 |                f_i__ghz      - Line center frequency, in GHz 
 |                delta_f__ghz  - From Equation 6
 |                delta         - From Equation 7
 |
 |      Returns:  F_i           - Line-shape factor
 |
 *===========================================================================*/
double LineShapeFactor(double f__ghz, double f_i__ghz, double delta_f__ghz, double delta)
{
    double term1 = f__ghz / f_i__ghz;
    double term2 = (delta_f__ghz - delta * (f_i__ghz - f__ghz)) / (pow(f_i__ghz - f__ghz, 2) + pow(delta_f__ghz, 2));
    double term3 = (delta_f__ghz - delta * (f_i__ghz + f__ghz)) / (pow(f_i__ghz + f__ghz, 2) + pow(delta_f__ghz, 2));

    double F_i = term1 * (term2 + term3);

    return F_i;
}
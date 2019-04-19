#include "..\include\p528.h"

/*=============================================================================
 |
 |  Description:  This function performs linear interpolation between the
 |                points (x1, y1) and (x2, y2).
 |
 |       Inputs:  (x1, y1)  - Point 1
 |                (x2, y2)  - Point 2
 |                x         - Value of the dependent variable
 |
 |      Returns:  y         - Linearly interpolated value
 |
 *===========================================================================*/
double LinearInterpolation(double x1, double y1, double x2, double y2, double x)
{
    return (y1 * (x2 - x) + y2 * (x - x1)) / (x2 - x1);
}
/** @file LinearInterpolation.cpp
 * Performs linear interpolation
 */
#include "p528.h"

/*******************************************************************************
 * Performs linear interpolation between the points (x1, y1) and (x2, y2).
 * @param[in] x1    x coordinate of Point 1
 * @param[in] y1    y coordinate of Point 1
 * @param[in] x2    x coordinate of Point 2
 * @param[in] y2    y coordinate of Point 2
 * @param[in] x     Value of the dependent variable
 * returns          Linearly interpolated value
 ******************************************************************************/
double LinearInterpolation(double x1, double y1, double x2, double y2, double x)
{
    return (y1 * (x2 - x) + y2 * (x - x1)) / (x2 - x1);
}

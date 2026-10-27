#include "p676.h"

namespace ITS {
namespace ITU {
namespace PSeries {
namespace p528 {

double WaterVapourDensityToPartialPressure(double rho__g_m3, double T__kelvin)
{
    // Equation 4
    double e__hPa = (rho__g_m3 * T__kelvin) / 216.7;

    return e__hPa;
}
}  // namespace p528
}  // namespace PSeries
}  // namespace ITU
}  // namespace ITS

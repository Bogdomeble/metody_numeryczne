      
#ifndef METEO_APPROXIMATION_HPP
#define METEO_APPROXIMATION_HPP

#include "common.hpp"       // Dla ValueSeries (wektor współczynników)
#include "integration.hpp"  // Dla IntegrableFunction (typ std::function)

namespace MeteoNumerical {
namespace Approximation {

Common::ValueSeries polynomialApproximation(
    int degree,
    double a,
    double b,
    const Integration::IntegrableFunction& func_to_approx,
    int integration_partitions = 100);

} // namespace Approximation
} // namespace MeteoNumerical

#endif // METEO_APPROXIMATION_HPP

    
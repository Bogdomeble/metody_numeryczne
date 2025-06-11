#ifndef METEO_COMMON_HPP
#define METEO_COMMON_HPP

#include <vector>
#include <string>
#include <numeric>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MeteoNumerical {
namespace Common {
    using ValueSeries = std::vector<double>;
    using Matrix = std::vector<std::vector<double>>;
    using IndexVector = std::vector<int>; // Dla permutacji w LU

    const double DEFAULT_EPSILON = 1e-12;

    inline double evaluatePolynomialHorner(const ValueSeries& coefficients, double x) {
        if (coefficients.empty()) {
            return 0.0;
        }
        double result = coefficients.back();
        for (int i = static_cast<int>(coefficients.size()) - 2; i >= 0; --i) {
            result = result * x + coefficients[i];
        }
        return result;
    }
} // namespace Common
} // namespace MeteoNumerical

#endif // METEO_COMMON_HPP
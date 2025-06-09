#ifndef METEO_ROOTFINDING_HPP
#define METEO_ROOTFINDING_HPP

#include "common.hpp"
#include <functional>

namespace MeteoNumerical {
namespace RootFinding {
    using RootFunction = std::function<double(double)>;

    inline double df_numeric(RootFunction func, double x, double h = 1e-7) {
        return (func(x + h) - func(x - h)) / (2.0 * h);
    }

    double newton_method_analytic(RootFunction func, RootFunction dfunc,
                                         double initial_guess, double tolerance, int max_iterations,
                                         Common::ValueSeries& iterations);

    double newton_method_numeric(RootFunction func,
                                         double initial_guess, double tolerance, int max_iterations,
                                         Common::ValueSeries& iterations);

    double secant_method(RootFunction func, double x0, double x1,
                                 double tolerance, int max_iterations,
                                 Common::ValueSeries& iterations);

    double bisection_method(RootFunction func, double a, double b,
                                    double tolerance, int max_iterations,
                                    Common::ValueSeries& iterations);

    double regula_falsi_method(RootFunction func, double a, double b,
                                       double tolerance, int max_iterations,
                                       Common::ValueSeries& iterations);
} // namespace RootFinding
} // namespace MeteoNumerical

#endif // METEO_ROOTFINDING_HPP
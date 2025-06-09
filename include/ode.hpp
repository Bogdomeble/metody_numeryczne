#ifndef METEO_ODE_HPP
#define METEO_ODE_HPP

#include "common.hpp"
#include <functional>
#include <vector>
#include <string>

namespace MeteoNumerical {
namespace ODE {
    using ODEFunction = std::function<double(double t, double y)>;
    using ODESolution = std::vector<std::pair<double, double>>;

    double eulerStep(ODEFunction func, double t_i, double y_i, double h);
    double heunStep(ODEFunction func, double t_i, double y_i, double h);
    double midpointStep(ODEFunction func, double t_i, double y_i, double h);
    double rk4Step(ODEFunction func, double t_i, double y_i, double h);

    ODESolution solve(ODEFunction func, double t0, double y0, double t_final, double h,
                            const std::string& method_name = "rk4");

    double getSolutionAtTime(const ODESolution& solution, double t_target);
} // namespace ODE
} // namespace MeteoNumerical

#endif // METEO_ODE_HPP
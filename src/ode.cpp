#include "ode.hpp"
#include <stdexcept>
#include <cmath>

namespace MeteoNumerical {
namespace ODE {

double eulerStep(ODEFunction func, double t_i, double y_i, double h) {
    if (std::isnan(y_i)) return y_i;
    return y_i + h * func(t_i, y_i);
}

double heunStep(ODEFunction func, double t_i, double y_i, double h) {
    if (std::isnan(y_i)) return y_i;
    double y_predictor = y_i + h * func(t_i, y_i);
    if (std::isnan(y_predictor)) y_predictor = y_i;
    return y_i + (h / 2.0) * (func(t_i, y_i) + func(t_i + h, y_predictor));
}

double midpointStep(ODEFunction func, double t_i, double y_i, double h) {
    if (std::isnan(y_i)) return y_i;
    double y_mid_arg = y_i + (h / 2.0) * func(t_i, y_i);
    if (std::isnan(y_mid_arg)) y_mid_arg = y_i;
    return y_i + h * func(t_i + h / 2.0, y_mid_arg);
}

double rk4Step(ODEFunction func, double t_i, double y_i, double h) {
    if (std::isnan(y_i)) return y_i;
    double k1 = h * func(t_i, y_i);
    double y_for_k2 = y_i + k1 / 2.0; if(std::isnan(y_for_k2)) y_for_k2 = y_i;
    double k2 = h * func(t_i + h / 2.0, y_for_k2);
    double y_for_k3 = y_i + k2 / 2.0; if(std::isnan(y_for_k3)) y_for_k3 = y_i;
    double k3 = h * func(t_i + h / 2.0, y_for_k3);
    double y_for_k4 = y_i + k3; if(std::isnan(y_for_k4)) y_for_k4 = y_i;
    double k4 = h * func(t_i + h, y_for_k4);
    return y_i + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
}

ODESolution solve(ODEFunction func, double t0, double y0, double t_final, double h, const std::string& method_name) {
    if (h == 0) throw std::runtime_error("ODE::solve: Step size h cannot be zero.");
    
    ODESolution results;
    double t = t0;
    double y = y0;
    results.push_back({t, y});

    std::function<double(ODEFunction, double, double, double)> step_func;
    if (method_name == "euler") step_func = eulerStep;
    else if (method_name == "heun") step_func = heunStep;
    else if (method_name == "midpoint") step_func = midpointStep;
    else if (method_name == "rk4") step_func = rk4Step;
    else throw std::runtime_error("ODE::solve: Unknown method_name: " + method_name);

    int num_steps = static_cast<int>(std::abs(t_final - t0) / std::abs(h));
    
    for(int i = 0; i < num_steps; ++i) {
        y = step_func(func, t, y, h);
        t += h;
        results.push_back({t, y});
    }

    if (std::abs(t - t_final) > Common::DEFAULT_EPSILON) {
         double last_h = t_final - t;
         if (std::abs(last_h) > Common::DEFAULT_EPSILON / 100.0) {
            y = step_func(func, t, y, last_h);
            t = t_final;
            results.push_back({t, y});
         }
    }
    return results;
}

double getSolutionAtTime(const ODESolution& solution, double t_target) {
    if (solution.empty()) return std::nan("");
    if (t_target < solution.front().first - Common::DEFAULT_EPSILON || t_target > solution.back().first + Common::DEFAULT_EPSILON) {
        return std::nan("");
    }
    for (size_t i = 0; i < solution.size(); ++i) {
        if (std::abs(solution[i].first - t_target) < Common::DEFAULT_EPSILON) {
            return solution[i].second;
        }
        if (solution[i].first > t_target && i > 0) {
            double t0 = solution[i-1].first;
            double y0 = solution[i-1].second;
            double t1 = solution[i].first;
            double y1 = solution[i].second;
            if (std::abs(t1 - t0) < Common::DEFAULT_EPSILON) return y0;
            return y0 + (y1 - y0) * (t_target - t0) / (t1 - t0);
        }
    }
    return solution.back().second;
}

} // namespace ODE
} // namespace MeteoNumerical
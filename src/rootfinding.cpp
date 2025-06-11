#include "rootfinding.hpp"
#include <stdexcept>
#include <iostream>
#include <limits>
#include <cmath>

namespace MeteoNumerical {
namespace RootFinding {

double newton_method_analytic(RootFunction func, RootFunction dfunc,
                                     double initial_guess, double tolerance, int max_iterations,
                                     Common::ValueSeries& iterations) {
    double x = initial_guess;
    iterations.clear();
    iterations.push_back(x);
    for (int i = 0; i < max_iterations; ++i) {
        double fx = func(x);
        double dfx = dfunc(x);
        if (std::abs(dfx) < Common::DEFAULT_EPSILON) {
            std::cerr << "Error: Newton (Analytic) - Derivative near zero." << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        double next_x = x - fx / dfx;
        iterations.push_back(next_x);
        if (std::abs(next_x - x) < tolerance || std::abs(fx) < tolerance) {
            return next_x;
        }
        x = next_x;
    }
    std::cerr << "Warning: Newton's method (analytic) did not converge." << std::endl;
    return x;
}

double newton_method_numeric(RootFunction func,
                                     double initial_guess, double tolerance, int max_iterations,
                                     Common::ValueSeries& iterations) {
    double x = initial_guess;
    iterations.clear();
    iterations.push_back(x);
    for (int i = 0; i < max_iterations; ++i) {
        double fx = func(x);
        double dfx = df_numeric(func, x);
        if (std::abs(dfx) < Common::DEFAULT_EPSILON) {
            std::cerr << "Error: Newton (Numeric) - Numerical derivative near zero." << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        double next_x = x - fx / dfx;
        iterations.push_back(next_x);
        if (std::abs(next_x - x) < tolerance || std::abs(fx) < tolerance) {
            return next_x;
        }
        x = next_x;
    }
    std::cerr << "Warning: Newton's method (numeric) did not converge." << std::endl;
    return x;
}

double secant_method(RootFunction func, double x0, double x1,
                             double tolerance, int max_iterations,
                             Common::ValueSeries& iterations) {
    iterations.clear();
    iterations.push_back(x0);
    iterations.push_back(x1);
    for (int i = 0; i < max_iterations; ++i) {
        double fx0 = func(x0);
        double fx1 = func(x1);
        if (std::abs(fx1 - fx0) < Common::DEFAULT_EPSILON) {
            std::cerr << "Warning: Secant - Difference of function values near zero." << std::endl;
            return x1;
        }
        double next_x = x1 - fx1 * (x1 - x0) / (fx1 - fx0);
        iterations.push_back(next_x);
        if (std::abs(next_x - x1) < tolerance || std::abs(func(next_x)) < tolerance) {
            return next_x;
        }
        x0 = x1;
        x1 = next_x;
    }
    std::cerr << "Warning: Secant method did not converge." << std::endl;
    return x1;
}

double bisection_method(RootFunction func, double a, double b,
                                double tolerance, int max_iterations,
                                Common::ValueSeries& iterations) {
    iterations.clear();
    double fa = func(a);
    if (fa * func(b) >= 0) {
        std::cerr << "Error: Bisection - Function has same signs at interval endpoints." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
    double m = a;
    for (int i = 0; i < max_iterations; ++i) {
        m = a + (b - a) / 2.0;
        iterations.push_back(m);
        double fm = func(m);
        if (std::abs(fm) < tolerance || (b - a) / 2.0 < tolerance) {
            return m;
        }
        if (fa * fm < 0) {
            b = m;
        } else {
            a = m;
            fa = fm;
        }
    }
    std::cerr << "Warning: Bisection method did not converge." << std::endl;
    return m;
}

double regula_falsi_method(RootFunction func, double a, double b,
                                   double tolerance, int max_iterations,
                                   Common::ValueSeries& iterations) {
    iterations.clear();
    double fa = func(a);
    double fb = func(b);
    if (fa * fb > 0) {
        std::cerr << "Error: Regula Falsi - Function has same signs at interval endpoints." << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
    double c = a;
    for (int i = 0; i < max_iterations; ++i) {
        if (std::abs(fb - fa) < Common::DEFAULT_EPSILON) {
             std::cerr << "Warning: Regula Falsi - f(b) - f(a) is too small." << std::endl;
             return c;
        }
        c = (a * fb - b * fa) / (fb - fa);
        iterations.push_back(c);
        double fc = func(c);
        if (std::abs(fc) < tolerance) {
            return c;
        }
        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }
    std::cerr << "Warning: Regula Falsi method did not converge." << std::endl;
    return c;
}

} // namespace RootFinding
} // namespace MeteoNumerical
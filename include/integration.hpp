#ifndef METEO_INTEGRATION_HPP
#define METEO_INTEGRATION_HPP

#include "common.hpp"
#include <functional>

namespace MeteoNumerical {
namespace Integration {
    using IntegrableFunction = std::function<double(double)>;

    double rectangleRule(IntegrableFunction f, double a, double b, int partitions);
    double trapezoidalRule(IntegrableFunction f, double a, double b, int partitions);
    double simpsonRule(IntegrableFunction f, double a, double b, int partitions);

    namespace GaussLegendre {
        double quadrature(IntegrableFunction func, double a, double b, int num_points);
        double composite(IntegrableFunction func, double a, double b, int num_points_per_interval, int num_partitions);
        // Uwaga: Stałe GL_NODES i GL_WEIGHTS zostaną umieszczone w pliku .cpp
    } // namespace GaussLegendre
} // namespace Integration
} // namespace MeteoNumerical

#endif // METEO_INTEGRATION_HPP
#include "integration.hpp"
#include <stdexcept>
#include <cmath>

namespace MeteoNumerical {
namespace Integration {

double rectangleRule(IntegrableFunction f, double a, double b, int partitions) {
    if (partitions <= 0) throw std::runtime_error("Partitions must be positive.");
    double sum = 0.0;
    double h = (b - a) / partitions;
    for (int i = 0; i < partitions; ++i) {
        sum += f(a + (i + 0.5) * h);
    }
    return sum * h;
}

double trapezoidalRule(IntegrableFunction f, double a, double b, int partitions) {
    if (partitions <= 0) throw std::runtime_error("Partitions must be positive.");
    double h = (b - a) / partitions;
    double sum = 0.5 * (f(a) + f(b));
    for (int i = 1; i < partitions; ++i) {
        sum += f(a + i * h);
    }
    return sum * h;
}

double simpsonRule(IntegrableFunction f, double a, double b, int partitions) {
    if (partitions <= 0) throw std::runtime_error("Partitions must be positive.");
    if (partitions % 2 != 0) partitions++;
    double h = (b - a) / partitions;
    double sum = f(a) + f(b);
    for (int i = 1; i < partitions; ++i) {
        sum += (i % 2 == 0 ? 2.0 : 4.0) * f(a + i * h);
    }
    return sum * h / 3.0;
}

namespace GaussLegendre {
    static const Common::Matrix GL_NODES = {
            {-0.5773502691708980,  0.5773502691708980},
            {-0.7745966692414834,  0.0,                 0.7745966692414834},
            {-0.8611363115940526, -0.3399810435848563,  0.3399810435848563,  0.8611363115940526},
            {-0.9061798459386640, -0.5384693101056831,  0.0,                 0.5384693101056831,  0.9061798459386640},
            {-0.9324695142031520, -0.6612093864662647, -0.2386191860831969,  0.2386191860831969,  0.6612093864662647,  0.9324695142031520}
    };
    static const Common::Matrix GL_WEIGHTS = {
            {1.0,                  1.0},
            {0.5555555555555556,   0.8888888888888889,  0.5555555555555556},
            {0.3478548451374538,   0.6521451548625461,  0.6521451548625461,  0.3478548451374538},
            {0.2369268850561891,   0.4786286704993662,  0.5688888888888889,  0.4786286704993662,  0.2369268850561891},
            {0.1713244923791704,   0.3607615730481386,  0.4679139345726910,  0.4679139345726910,  0.3607615730481386,  0.1713244923791704}
    };

    double quadrature(IntegrableFunction func, double a, double b, int num_points) {
        if (num_points < 2 || num_points > 6) {
            throw std::out_of_range("GaussLegendre::quadrature: num_points must be between 2 and 6.");
        }
        int idx = num_points - 2;
        double sum = 0.0;
        double transform_coeff1 = 0.5 * (b - a);
        double transform_coeff2 = 0.5 * (a + b);
        for (int i = 0; i < num_points; ++i) {
            double x_transformed = transform_coeff1 * GL_NODES[idx][i] + transform_coeff2;
            sum += GL_WEIGHTS[idx][i] * func(x_transformed);
        }
        return sum * transform_coeff1;
    }

    double composite(IntegrableFunction func, double a, double b, int num_points_per_interval, int num_partitions) {
        if (num_partitions <= 0) throw std::runtime_error("Partitions must be positive.");
        double interval_size = (b - a) / num_partitions;
        double total_sum = 0.0;
        for (int p = 0; p < num_partitions; ++p) {
            double sub_a = a + p * interval_size;
            double sub_b = sub_a + interval_size;
            total_sum += quadrature(func, sub_a, sub_b, num_points_per_interval);
        }
        return total_sum;
    }
} // namespace GaussLegendre

} // namespace Integration
} // namespace MeteoNumerical
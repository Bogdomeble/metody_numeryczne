#include "interpolation.hpp"
#include <stdexcept>
#include <cmath>

namespace MeteoNumerical {
namespace Interpolation {

namespace Newton {

Common::Matrix calculateDividedDifferences(const Common::ValueSeries& x, const Common::ValueSeries& y) {
    if (x.size() != y.size() || x.empty()) {
        throw std::runtime_error("Newton::calculateDividedDifferences: x and y must have the same non-zero size.");
    }
    size_t n = x.size();
    Common::Matrix divDiff(n, Common::ValueSeries(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        divDiff[i][0] = y[i];
    }
    for (size_t j = 1; j < n; ++j) {
        for (size_t i = 0; i < n - j; ++i) {
            if (std::abs(x[i + j] - x[i]) < Common::DEFAULT_EPSILON) {
                throw std::runtime_error("Newton::calculateDividedDifferences: duplicate x values encountered, division by zero.");
            }
            divDiff[i][j] = (divDiff[i + 1][j - 1] - divDiff[i][j - 1]) / (x[i + j] - x[i]);
        }
    }
    return divDiff;
}

Common::ValueSeries getNewtonCoefficients(const Common::Matrix& divDiff) {
    if (divDiff.empty() || divDiff[0].empty()) {
        throw std::runtime_error("Newton::getNewtonCoefficients: divided differences table is empty.");
    }
    Common::ValueSeries coeffs;
    coeffs.reserve(divDiff[0].size());
    for (size_t i = 0; i < divDiff[0].size(); ++i) {
        coeffs.push_back(divDiff[0][i]);
    }
    return coeffs;
}

double newtonInterpolate(const Common::ValueSeries& x_nodes, const Common::ValueSeries& newton_coeffs, double xp) {
    if (x_nodes.empty() || newton_coeffs.empty()) {
        throw std::runtime_error("Newton::newtonInterpolate: node or coefficient vectors are empty.");
    }
    size_t n_coeffs = newton_coeffs.size();
    if (n_coeffs == 0) return std::nan("");
    double result = newton_coeffs[0];
    double term = 1.0;
    for (size_t i = 1; i < n_coeffs; ++i) {
        if (i > x_nodes.size()) {
            throw std::out_of_range("Newton::newtonInterpolate: not enough x_nodes for the given coefficients.");
        }
        term *= (xp - x_nodes[i - 1]);
        result += newton_coeffs[i] * term;
    }
    return result;
}

} // namespace Newton

namespace Lagrange {

double lagrangeInterpolate(const Common::ValueSeries& x_nodes, const Common::ValueSeries& y_nodes, double xp) {
    if (x_nodes.size() != y_nodes.size() || x_nodes.empty()) {
        throw std::runtime_error("Lagrange::lagrangeInterpolate: x_nodes and y_nodes must have the same non-zero size.");
    }
    double yp = 0.0;
    size_t n = x_nodes.size();
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(xp - x_nodes[i]) < Common::DEFAULT_EPSILON) {
            return y_nodes[i];
        }
        double L_i = 1.0;
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            if (std::abs(x_nodes[i] - x_nodes[j]) < Common::DEFAULT_EPSILON) {
                throw std::runtime_error("Lagrange::lagrangeInterpolate: duplicate x_nodes encountered, division by zero.");
            }
            L_i *= (xp - x_nodes[j]) / (x_nodes[i] - x_nodes[j]);
        }
        yp += L_i * y_nodes[i];
    }
    return yp;
}

} // namespace Lagrange

void selectNodesByStep(const Common::ValueSeries& x_all, const Common::ValueSeries& y_all,
                              Common::ValueSeries& x_nodes, Common::ValueSeries& y_nodes, unsigned int k_step) {
    if (x_all.size() != y_all.size()) {
        throw std::runtime_error("selectNodesByStep: x_all and y_all must have the same size.");
    }
    if (k_step == 0) {
        throw std::runtime_error("selectNodesByStep: k_step must be positive.");
    }
    x_nodes.clear();
    y_nodes.clear();
    for (size_t i = 0; i < x_all.size(); i += k_step) {
        x_nodes.push_back(x_all[i]);
        y_nodes.push_back(y_all[i]);
    }
}

double calculateInterpolationMSE(
        const Common::ValueSeries& x_all, const Common::ValueSeries& y_all,
        const Common::ValueSeries& x_nodes, const Common::ValueSeries& y_nodes_or_coeffs,
        InterpolatorFunc interpolator,
        Common::ValueSeries* out_mse_vector,
        Common::ValueSeries* out_x_error_points) {
    if (x_all.size() != y_all.size() || x_all.empty()) {
        throw std::runtime_error("calculateInterpolationMSE: x_all and y_all are invalid.");
    }
    double total_mse_sum_sq_error = 0.0;
    int count = 0;
    if (out_mse_vector) out_mse_vector->clear();
    if (out_x_error_points) out_x_error_points->clear();
    for (size_t i = 0; i < x_all.size(); ++i) {
        double y_interp = interpolator(x_nodes, y_nodes_or_coeffs, x_all[i]);
        double error = y_interp - y_all[i];
        double squared_error = error * error;
        total_mse_sum_sq_error += squared_error;
        count++;
        if (out_mse_vector) out_mse_vector->push_back(squared_error);
        if (out_x_error_points) out_x_error_points->push_back(x_all[i]);
    }
    return (count > 0) ? total_mse_sum_sq_error / count : 0.0;
}

} // namespace Interpolation
} // namespace MeteoNumerical
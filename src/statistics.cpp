#include "statistics.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace MeteoNumerical {
namespace Statistics {

double calculate_average(const Common::ValueSeries& values) {
    if (values.empty()) return std::nan("");
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}

double find_min(const Common::ValueSeries& values) {
    if (values.empty()) return std::nan("");
    return *std::min_element(values.begin(), values.end());
}

double find_max(const Common::ValueSeries& values) {
    if (values.empty()) return std::nan("");
    return *std::max_element(values.begin(), values.end());
}

double calculate_std_dev(const Common::ValueSeries& values, bool population) {
    if (values.size() < 2) return std::nan("");
    double mean = calculate_average(values);
    double sq_sum_diff = 0.0;
    for (double val : values) {
        sq_sum_diff += std::pow(val - mean, 2);
    }
    if (population) {
        return std::sqrt(sq_sum_diff / values.size());
    } else {
        return std::sqrt(sq_sum_diff / (values.size() - 1));
    }
}

double calculate_variance(const Common::ValueSeries& values, bool population) {
    if (values.size() < 2) return std::nan("");
    double std_dev = calculate_std_dev(values, population);
    return std::pow(std_dev, 2);
}

double calculate_mse(const Common::ValueSeries& y_true, const Common::ValueSeries& y_pred) {
    if (y_true.size() != y_pred.size() || y_true.empty()) {
        return std::nan("");
    }
    double mse = 0.0;
    for (size_t i = 0; i < y_true.size(); ++i) {
        mse += std::pow(y_true[i] - y_pred[i], 2);
    }
    return mse / y_true.size();
}

} // namespace Statistics
} // namespace MeteoNumerical
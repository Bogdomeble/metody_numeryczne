#ifndef METEO_STATISTICS_HPP
#define METEO_STATISTICS_HPP

#include "common.hpp"

namespace MeteoNumerical {
namespace Statistics {
    double calculate_average(const Common::ValueSeries& values);
    double find_min(const Common::ValueSeries& values);
    double find_max(const Common::ValueSeries& values);
    double calculate_std_dev(const Common::ValueSeries& values, bool population = false);
    double calculate_variance(const Common::ValueSeries& values, bool population = false);
    double calculate_mse(const Common::ValueSeries& y_true, const Common::ValueSeries& y_pred);
} // namespace Statistics
} // namespace MeteoNumerical

#endif // METEO_STATISTICS_HPP
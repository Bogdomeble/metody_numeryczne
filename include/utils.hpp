#ifndef METEO_UTILS_HPP
#define METEO_UTILS_HPP

#include "common.hpp"
#include "datastructures.hpp"

namespace MeteoNumerical {
namespace Utils {
    inline Common::ValueSeries extractTemperatures(const DataStructures::MeasurementSeries& series) {
        Common::ValueSeries temps; temps.reserve(series.size());
        for (const auto& m : series) temps.push_back(m.temperature_celsius);
        return temps;
    }
    inline Common::ValueSeries extractHumidities(const DataStructures::MeasurementSeries& series) {
        Common::ValueSeries hums; hums.reserve(series.size());
        for (const auto& m : series) hums.push_back(m.humidity_percent);
        return hums;
    }
} // namespace Utils
} // namespace MeteoNumerical

#endif // METEO_UTILS_HPP
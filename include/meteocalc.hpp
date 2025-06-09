#ifndef METEO_METEOCALC_HPP
#define METEO_METEOCALC_HPP

#include "conversions.hpp"
#include <cmath>
#include <stdexcept>

namespace MeteoNumerical {
namespace MeteoCalculations {
    inline double calculate_dew_point(double temperature_celsius, double relative_humidity_percent) {
        if (relative_humidity_percent < 0.0 || relative_humidity_percent > 100.0) {
            throw std::out_of_range("Relative humidity must be between 0 and 100.");
        }
        const double a = 17.27; const double b = 237.7;
        double alpha = ((a * temperature_celsius) / (b + temperature_celsius)) + std::log(relative_humidity_percent / 100.0);
        return (b * alpha) / (a - alpha);
    }
    inline double calculate_wind_chill(double temperature_celsius, double wind_speed_mps) {
        double wind_speed_kmh = Conversions::mps_to_kmh(wind_speed_mps);
        if (temperature_celsius >= 10.0 || wind_speed_kmh <= 4.8) return temperature_celsius;
        return 13.12 + 0.6215 * temperature_celsius - 11.37 * std::pow(wind_speed_kmh, 0.16) +
               0.3965 * temperature_celsius * std::pow(wind_speed_kmh, 0.16);
    }
} // namespace MeteoCalculations
} // namespace MeteoNumerical

#endif // METEO_METEOCALC_HPP
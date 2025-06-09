#ifndef METEO_DATASTRUCTURES_HPP
#define METEO_DATASTRUCTURES_HPP

#include <vector>
#include <cmath> // dla NAN

namespace MeteoNumerical {
namespace DataStructures {
    struct Measurement {
        double temperature_celsius;
        double humidity_percent;
        double pressure_hpa;
        double wind_speed_mps;
        Measurement(double temp = NAN, double hum = NAN, double press = NAN, double wind = NAN)
                : temperature_celsius(temp), humidity_percent(hum), pressure_hpa(press), wind_speed_mps(wind) {}
    };
    using MeasurementSeries = std::vector<Measurement>;
} // namespace DataStructures
} // namespace MeteoNumerical

#endif // METEO_DATASTRUCTURES_HPP
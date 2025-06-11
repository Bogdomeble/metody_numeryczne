#ifndef METEO_CONVERSIONS_HPP
#define METEO_CONVERSIONS_HPP

namespace MeteoNumerical {
namespace Conversions {
    inline double celsius_to_fahrenheit(double celsius) { return (celsius * 9.0 / 5.0) + 32.0; }
    inline double fahrenheit_to_celsius(double fahrenheit) { return (fahrenheit - 32.0) * 5.0 / 9.0; }
    inline double mps_to_kmh(double mps) { return mps * 3.6; }
    inline double kmh_to_mps(double kmh) { return kmh / 3.6; }
    inline double hpa_to_mmhg(double hpa) { return hpa * 0.750062; }
    inline double mmhg_to_hpa(double mmhg) { return mmhg / 0.750062; }
} // namespace Conversions
} // namespace MeteoNumerical

#endif // METEO_CONVERSIONS_HPP
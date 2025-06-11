#ifndef METEO_FILEIO_HPP
#define METEO_FILEIO_HPP

#include "common.hpp"
#include <string>
#include <utility> // For std::pair
#include <algorithm> // for replace, remove_if

namespace MeteoNumerical {
namespace FileIO {
    std::pair<Common::ValueSeries, Common::ValueSeries> readXYDataFromLines(const std::string& filename,
                                                                                   const std::string& x_label = "xi:",
                                                                                   const std::string& y_label = "f(xi):");

    void writeXYDataToCSV(const std::string& filename,
                                 const Common::ValueSeries& x_data,
                                 const Common::ValueSeries& y_data,
                                 const std::string& x_header = "x",
                                 const std::string& y_header = "y");

    inline std::string sanitize_filename(const std::string& input) {
        std::string output = input;
        std::replace(output.begin(), output.end(), ' ', '_');
        output.erase(std::remove_if(output.begin(), output.end(),
            [](char c) { return std::string("[]()/\\:*?\"<>|").find(c) != std::string::npos; }),
            output.end());
        std::replace(output.begin(), output.end(), '.', '_');
        return output;
    }
    
    void saveRootFindingResultsToCSV(const std::string& method_name, const std::string& func_name_str,
                                     const Common::ValueSeries& iterations,
                                     double found_root,
                                     double true_root_for_error_calc = std::numeric_limits<double>::quiet_NaN());

} // namespace FileIO
} // namespace MeteoNumerical

#endif // METEO_FILEIO_HPP
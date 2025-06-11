#include "fileio.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <limits>

namespace MeteoNumerical {
namespace FileIO {

std::pair<Common::ValueSeries, Common::ValueSeries> readXYDataFromLines(const std::string& filename,
                                                                               const std::string& x_label,
                                                                               const std::string& y_label) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("FileIO::readXYDataFromLines: Error opening file: " + filename);
    }
    std::string line;
    Common::ValueSeries x_data, y_data;
    
    // Skip header
    if (!std::getline(file, line)) {
        throw std::runtime_error("FileIO::readXYDataFromLines: File is empty or failed to read header line.");
    }

    // Read X data
    if (!std::getline(file, line)) {
        throw std::runtime_error("FileIO::readXYDataFromLines: Missing X data line.");
    }
    size_t x_pos = line.find(x_label);
    if (x_pos != std::string::npos) {
        std::istringstream stream(line.substr(x_pos + x_label.length()));
        double val;
        while (stream >> val) x_data.push_back(val);
    } else {
        throw std::runtime_error("FileIO::readXYDataFromLines: X data label not found.");
    }

    // Read Y data
    if (!std::getline(file, line)) {
        throw std::runtime_error("FileIO::readXYDataFromLines: Missing Y data line.");
    }
    size_t y_pos = line.find(y_label);
    if (y_pos != std::string::npos) {
        std::istringstream stream(line.substr(y_pos + y_label.length()));
        double val;
        while (stream >> val) y_data.push_back(val);
    } else {
        throw std::runtime_error("FileIO::readXYDataFromLines: Y data label not found.");
    }

    if (x_data.size() != y_data.size()){
        throw std::runtime_error("FileIO::readXYDataFromLines: X and Y data series have different lengths.");
    }
    file.close();
    return {x_data, y_data};
}

void writeXYDataToCSV(const std::string& filename,
                             const Common::ValueSeries& x_data,
                             const Common::ValueSeries& y_data,
                             const std::string& x_header,
                             const std::string& y_header) {
    if (x_data.size() != y_data.size()) {
        throw std::runtime_error("FileIO::writeXYDataToCSV: x_data and y_data must have the same size.");
    }
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("FileIO::writeXYDataToCSV: Could not open output file: " + filename);
    }
    outFile << x_header << "," << y_header << "\n";
    outFile << std::fixed << std::setprecision(8);
    for (size_t i = 0; i < x_data.size(); ++i) {
        outFile << x_data[i] << "," << y_data[i] << "\n";
    }
    outFile.close();
}

void saveRootFindingResultsToCSV(const std::string& method_name, const std::string& func_name_str,
                                 const Common::ValueSeries& iterations,
                                 double found_root,
                                 double true_root_for_error_calc) {
    if (iterations.empty() && std::isnan(found_root)) {
        return;
    }

    std::string csv_filename = sanitize_filename(method_name) + "_" + sanitize_filename(func_name_str) + "_iterations.csv";
    std::ofstream outfile(csv_filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open CSV file " << csv_filename << " for writing." << std::endl;
        return;
    }

    outfile << "Iteration_idx,Approximation_xk";
    bool has_true_root = !std::isnan(true_root_for_error_calc);
    if (has_true_root) {
        outfile << ",AbsoluteError_vs_TrueRoot";
    }
    outfile << "\n";
    outfile << std::fixed << std::setprecision(15);

    if (!iterations.empty()) {
        for (size_t i = 0; i < iterations.size(); ++i) {
            outfile << i << "," << iterations[i];
            if (has_true_root) {
                outfile << "," << std::abs(iterations[i] - true_root_for_error_calc);
            }
            outfile << "\n";
        }
    } else if (!std::isnan(found_root)) {
        outfile << 0 << "," << found_root;
        if (has_true_root) {
             outfile << "," << std::abs(found_root - true_root_for_error_calc);
        }
        outfile << "\n";
    }
    outfile.close();
}

} // namespace FileIO
} // namespace MeteoNumerical
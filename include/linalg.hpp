#ifndef METEO_LINALG_HPP
#define METEO_LINALG_HPP

#include "common.hpp"
#include <iostream>

namespace MeteoNumerical {
namespace LinearAlgebra {
    void printMatrix(const Common::Matrix& matrix, std::ostream& os = std::cout, int precision = 5);
    void printVector(const Common::ValueSeries& vec, std::ostream& os = std::cout, int precision = 5);

    Common::ValueSeries gaussElimination(Common::Matrix A, Common::ValueSeries b);
    
    bool luDecompositionPivoting(const Common::Matrix& A, Common::Matrix& L, Common::Matrix& U, Common::IndexVector& P);
    
    Common::ValueSeries permuteVector(const Common::ValueSeries& b, const Common::IndexVector& P);
    Common::ValueSeries forwardSubstitution(const Common::Matrix& L, const Common::ValueSeries& pb);
    Common::ValueSeries backwardSubstitution(const Common::Matrix& U, const Common::ValueSeries& y);
    
    Common::ValueSeries solveWithLU(const Common::Matrix& A, const Common::ValueSeries& b);

    Common::Matrix multiplyMatrices(const Common::Matrix& A, const Common::Matrix& B);
} // namespace LinearAlgebra
} // namespace MeteoNumerical

#endif // METEO_LINALG_HPP
#include "linalg.hpp"
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iomanip>

namespace MeteoNumerical {
namespace LinearAlgebra {

void printMatrix(const Common::Matrix& matrix, std::ostream& os, int precision) {
    for (const auto& row : matrix) {
        for (double val : row) {
            os << std::fixed << std::setw(10 + precision) << std::setprecision(precision) << val << " ";
        }
        os << std::endl;
    }
    os << std::endl;
}

void printVector(const Common::ValueSeries& vec, std::ostream& os, int precision) {
    for (double val : vec) {
        os << std::fixed << std::setw(10 + precision) << std::setprecision(precision) << val << " ";
    }
    os << std::endl;
}

Common::ValueSeries gaussElimination(Common::Matrix A, Common::ValueSeries b) {
    size_t n = A.size(); // Używamy size_t
    if (n == 0 || A[0].size() != n || b.size() != n) {
        throw std::runtime_error("GaussElimination: Invalid matrix or vector dimensions.");
    }
    Common::Matrix augmented(n, Common::ValueSeries(n + 1));
    for (size_t i = 0; i < n; ++i) { // Używamy size_t
        for (size_t j = 0; j < n; ++j) { // Używamy size_t
            augmented[i][j] = A[i][j];
        }
        augmented[i][n] = b[i];
    }
    for (size_t k = 0; k < n; ++k) { // Używamy size_t
        size_t max_row_idx = k; // Używamy size_t
        for (size_t i = k + 1; i < n; ++i) { // Używamy size_t
            if (std::abs(augmented[i][k]) > std::abs(augmented[max_row_idx][k])) {
                max_row_idx = i;
            }
        }
        std::swap(augmented[k], augmented[max_row_idx]);
        if (std::abs(augmented[k][k]) < Common::DEFAULT_EPSILON) {
            throw std::runtime_error("GaussElimination: Matrix is singular.");
        }
        for (size_t i = k + 1; i < n; ++i) { // Używamy size_t
            double factor = augmented[i][k] / augmented[k][k];
            for (size_t j = k; j <= n; ++j) { // Używamy size_t
                augmented[i][j] -= factor * augmented[k][j];
            }
        }
    }
    Common::ValueSeries x(n);
    for (int i = n - 1; i >= 0; --i) { // Tutaj int jest OK, bo idziemy w dół
        x[i] = augmented[i][n];
        for (size_t j = i + 1; j < n; ++j) { // Używamy size_t
            x[i] -= augmented[i][j] * x[j];
        }
        x[i] /= augmented[i][i];
    }
    return x;
}

bool luDecompositionPivoting(const Common::Matrix& A, Common::Matrix& L, Common::Matrix& U, Common::IndexVector& P) {
    size_t n = A.size(); // Używamy size_t
    if (n == 0 || A[0].size() != n) return false;
    U = A;
    L.assign(n, Common::ValueSeries(n, 0.0));
    P.resize(n);
    std::iota(P.begin(), P.end(), 0);
    for (size_t k = 0; k < n; ++k) { // Używamy size_t
        L[k][k] = 1.0;
        size_t maxRow_idx = k; // Używamy size_t
        for (size_t i = k + 1; i < n; ++i) { // Używamy size_t
            if (std::abs(U[i][k]) > std::abs(U[maxRow_idx][k])) {
                maxRow_idx = i;
            }
        }
        if (std::abs(U[maxRow_idx][k]) < Common::DEFAULT_EPSILON) {
            return false;
        }
        if (maxRow_idx != k) {
            std::swap(U[k], U[maxRow_idx]);
            std::swap(P[k], P[maxRow_idx]);
            for (size_t j = 0; j < k; ++j) { // Używamy size_t
                std::swap(L[k][j], L[maxRow_idx][j]);
            }
        }
        for (size_t i = k + 1; i < n; ++i) { // Używamy size_t
            L[i][k] = U[i][k] / U[k][k];
            for (size_t j = k; j < n; ++j) { // Używamy size_t
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
    }
    return true;
}

Common::ValueSeries permuteVector(const Common::ValueSeries& b, const Common::IndexVector& P) {
    if (b.size() != P.size()) throw std::runtime_error("permuteVector: b and P sizes mismatch.");
    Common::ValueSeries pb(b.size());
    for (size_t i = 0; i < b.size(); ++i) { // Używamy size_t
        pb[i] = b[P[i]];
    }
    return pb;
}

Common::ValueSeries forwardSubstitution(const Common::Matrix& L, const Common::ValueSeries& pb) {
    size_t n = L.size(); // Używamy size_t
    if (n == 0 || L[0].size() != n || pb.size() != n) {
        throw std::runtime_error("forwardSubstitution: Invalid matrix/vector dimensions.");
    }
    Common::ValueSeries y(n);
    for (size_t i = 0; i < n; ++i) { // Używamy size_t
        y[i] = pb[i];
        for (size_t j = 0; j < i; ++j) { // Używamy size_t
            y[i] -= L[i][j] * y[j];
        }
    }
    return y;
}

Common::ValueSeries backwardSubstitution(const Common::Matrix& U, const Common::ValueSeries& y) {
    size_t n = U.size(); // Używamy size_t
    if (n == 0 || U[0].size() != n || y.size() != n) {
        throw std::runtime_error("backwardSubstitution: Invalid matrix/vector dimensions.");
    }
    Common::ValueSeries x(n);
    for (int i = n - 1; i >= 0; --i) { // Tutaj int jest OK, bo idziemy w dół
        if (std::abs(U[i][i]) < Common::DEFAULT_EPSILON) {
            throw std::runtime_error("backwardSubstitution: Division by zero, singular U matrix.");
        }
        x[i] = y[i];
        for (size_t j = i + 1; j < n; ++j) { // Używamy size_t
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
    return x;
}

Common::ValueSeries solveWithLU(const Common::Matrix& A, const Common::ValueSeries& b) {
    Common::Matrix L, U;
    Common::IndexVector P;
    if (!luDecompositionPivoting(A, L, U, P)) {
        throw std::runtime_error("solveWithLU: LU decomposition failed (likely singular matrix).");
    }
    Common::ValueSeries pb = permuteVector(b, P);
    Common::ValueSeries y = forwardSubstitution(L, pb);
    Common::ValueSeries x = backwardSubstitution(U, y);
    return x;
}

Common::Matrix multiplyMatrices(const Common::Matrix& A, const Common::Matrix& B) {
    if (A.empty() || B.empty() || A[0].size() != B.size()) {
        throw std::runtime_error("multiplyMatrices: Incompatible matrix dimensions for multiplication.");
    }
    size_t rowsA = A.size();
    size_t colsA = A[0].size();
    size_t colsB = B[0].size();
    Common::Matrix result(rowsA, Common::ValueSeries(colsB, 0.0));
    for (size_t i = 0; i < rowsA; ++i) {
        for (size_t j = 0; j < colsB; ++j) {
            for (size_t k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

} // namespace LinearAlgebra
} // namespace MeteoNumerical
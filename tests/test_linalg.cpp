#include "gtest/gtest.h"
#include "linalg.hpp"
#include <stdexcept>

using namespace MeteoNumerical;

TEST(LinearAlgebraTest, SolveWithLUSuccess) {
    // Przypadek poprawny: Rozwiązywalny układ równań.
    Common::Matrix A = {{2, 1}, {1, 3}};
    Common::ValueSeries b = {4, 7};
    Common::ValueSeries expected_x = {1.0, 2.0};
    
    Common::ValueSeries x = LinearAlgebra::solveWithLU(A, b);

    // POPRAWKA: Używamy literału '2u' (unsigned), aby typy się zgadzały.
    ASSERT_EQ(x.size(), 2u);
    EXPECT_NEAR(x[0], expected_x[0], 1e-9);
    EXPECT_NEAR(x[1], expected_x[1], 1e-9);
}

TEST(LinearAlgebraTest, SolveWithLUThrowsOnSingularMatrix) {
    // Przypadek błędny: Nierozwiązywalny układ (macierz osobliwa).
    Common::Matrix A = {{1, 1}, {1, 1}}; // Rzędy są zależne liniowo
    Common::ValueSeries b = {1, 2};

    // Oczekujemy, że funkcja rzuci wyjątek std::runtime_error.
    EXPECT_THROW(LinearAlgebra::solveWithLU(A, b), std::runtime_error);
}

TEST(LinearAlgebraTest, GaussEliminationSimple2x2) {
    MeteoNumerical::Common::Matrix A = {{2, 1}, {1, 3}};
    MeteoNumerical::Common::ValueSeries b = {4, 7};
    MeteoNumerical::Common::ValueSeries x = MeteoNumerical::LinearAlgebra::gaussElimination(A, b);
    
    EXPECT_NEAR(x[0], 1.0, 1e-9);
    EXPECT_NEAR(x[1], 2.0, 1e-9);
}

TEST(LinearAlgebraTest, GaussEliminationThrowsOnInconsistentSystem) {
    MeteoNumerical::Common::Matrix A = {{2, 1}, {4, 2}}; // Układ sprzeczny
    MeteoNumerical::Common::ValueSeries b = {4, 7};
    
    EXPECT_THROW(MeteoNumerical::LinearAlgebra::gaussElimination(A, b), std::runtime_error);
}

TEST(LinearAlgebraTest, MatrixMultiplication) {
    MeteoNumerical::Common::Matrix A = {{1, 2}, {3, 4}};
    MeteoNumerical::Common::Matrix B = {{5, 6}, {7, 8}};
    MeteoNumerical::Common::Matrix expected = {{19, 22}, {43, 50}};

    MeteoNumerical::Common::Matrix C = MeteoNumerical::LinearAlgebra::multiplyMatrices(A, B);
    
    EXPECT_EQ(C.size(), 2);
    EXPECT_EQ(C[0].size(), 2);
    EXPECT_NEAR(C[0][0], expected[0][0], 1e-9);
    EXPECT_NEAR(C[0][1], expected[0][1], 1e-9);
    EXPECT_NEAR(C[1][0], expected[1][0], 1e-9);
    EXPECT_NEAR(C[1][1], expected[1][1], 1e-9);
}

TEST(LinearAlgebraTest, MatrixMultiplicationThrowsOnDimensionMismatch) {
    MeteoNumerical::Common::Matrix A = {{1, 2}, {3, 4}}; // 2x2
    MeteoNumerical::Common::Matrix B = {{1}, {2}, {3}};  // 3x1
    
    EXPECT_THROW(MeteoNumerical::LinearAlgebra::multiplyMatrices(A, B), std::runtime_error);
}
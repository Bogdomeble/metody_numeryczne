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
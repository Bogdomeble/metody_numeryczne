#include "gtest/gtest.h"
#include "approximation.hpp" // Testowany moduł
#include <cmath>
#include <vector>

// Tolerancja dla porównań liczb zmiennoprzecinkowych
constexpr double tolerance = 1e-8;

// ----- Test Suite dla Modułu Aproksymacji -----

// Test 1: Sprawdza, czy funkcja poprawnie aproksymuje stałą funkcję.
// f(x) = 5.0. Wielomian stopnia 0 powinien mieć jeden współczynnik: a0 = 5.0.
TEST(ApproximationTest, ApproximatesConstantFunctionCorrectly) {
    auto constant_func = [](double x) { return 5.0; };
    int degree = 0;
    double a = 0.0, b = 10.0;

    MeteoNumerical::Common::ValueSeries coeffs = 
        MeteoNumerical::Approximation::polynomialApproximation(degree, a, b, constant_func);

    // Oczekujemy jednego współczynnika
    ASSERT_EQ(coeffs.size(), 1);
    // Ten współczynnik powinien być bliski 5.0
    EXPECT_NEAR(coeffs[0], 5.0, tolerance);
}

// Test 2: Sprawdza, czy funkcja (prawie) idealnie aproksymuje funkcję liniową
// wielomianem tego samego stopnia. f(x) = 2x - 3.
// Wielomian stopnia 1 powinien mieć współczynniki: a0 = -3.0, a1 = 2.0.
TEST(ApproximationTest, ApproximatesLinearFunctionExactly) {
    auto linear_func = [](double x) { return 2.0 * x - 3.0; };
    int degree = 1;
    double a = -5.0, b = 5.0;

    MeteoNumerical::Common::ValueSeries coeffs = 
        MeteoNumerical::Approximation::polynomialApproximation(degree, a, b, linear_func, 200); // Więcej partycji dla dokładności

    // Oczekujemy dwóch współczynników [a0, a1]
    ASSERT_EQ(coeffs.size(), 2);
    EXPECT_NEAR(coeffs[0], -3.0, tolerance); // a0
    EXPECT_NEAR(coeffs[1], 2.0, tolerance);  // a1
}

// Test 3: Sprawdza, czy funkcja (prawie) idealnie aproksymuje funkcję kwadratową.
// f(x) = x^2 - 4x + 4.
// Wielomian stopnia 2 powinien mieć wsp.: a0=4, a1=-4, a2=1.
TEST(ApproximationTest, ApproximatesQuadraticFunctionExactly) {
    auto quadratic_func = [](double x) { return x * x - 4.0 * x + 4.0; };
    int degree = 2;
    double a = 0.0, b = 10.0;

    MeteoNumerical::Common::ValueSeries coeffs = 
        MeteoNumerical::Approximation::polynomialApproximation(degree, a, b, quadratic_func, 200);

    // Oczekujemy trzech współczynników [a0, a1, a2]
    ASSERT_EQ(coeffs.size(), 3);
    EXPECT_NEAR(coeffs[0], 4.0, tolerance);  // a0
    EXPECT_NEAR(coeffs[1], -4.0, tolerance); // a1
    EXPECT_NEAR(coeffs[2], 1.0, tolerance);  // a2
}

// Test 4: Sprawdza, czy funkcja rzuca wyjątek dla nieprawidłowego (ujemnego) stopnia.
TEST(ApproximationTest, ThrowsForNegativeDegree) {
    auto any_func = [](double x) { return x; };
    int degree = -1;
    double a = 0.0, b = 1.0;

    // Oczekujemy, że wywołanie funkcji z ujemnym stopniem rzuci wyjątek std::runtime_error
    EXPECT_THROW(
        MeteoNumerical::Approximation::polynomialApproximation(degree, a, b, any_func),
        std::runtime_error
    );
}

// Test 5: Sprawdza zachowanie dla przedziału o zerowej długości.
// W takim przypadku macierz A będzie osobliwa. Oczekujemy rzucenia wyjątku przez solwer.
TEST(ApproximationTest, ThrowsForZeroLengthInterval) {
    auto any_func = [](double x) { return x; };
    int degree = 2;
    double a = 5.0, b = 5.0;

    // Funkcja solveWithLU powinna rzucić wyjątek, ponieważ macierz będzie osobliwa
    // (wszystkie całki będą równe zero).
    EXPECT_THROW(
        MeteoNumerical::Approximation::polynomialApproximation(degree, a, b, any_func),
        std::runtime_error
    );
}
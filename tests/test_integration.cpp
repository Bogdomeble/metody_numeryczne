#include "gtest/gtest.h"
#include "integration.hpp"
#include <cmath>

// Funkcja do testowania: f(x) = x^2
// Całka od 0 do 3 z x^2 dx = [x^3 / 3] od 0 do 3 = 27/3 = 9
double square(double x) {
    return x * x;
}

TEST(IntegrationTest, RectangleRule) {
    double result = MeteoNumerical::Integration::rectangleRule(square, 0.0, 3.0, 1000);
    EXPECT_NEAR(result, 9.0, 1e-3);
}

TEST(IntegrationTest, TrapezoidalRule) {
    double result = MeteoNumerical::Integration::trapezoidalRule(square, 0.0, 3.0, 1000);
    EXPECT_NEAR(result, 9.0, 1e-5);
}

TEST(IntegrationTest, SimpsonRule) {
    double result = MeteoNumerical::Integration::simpsonRule(square, 0.0, 3.0, 1000);
    EXPECT_NEAR(result, 9.0, 1e-9);
}

TEST(IntegrationTest, SimpsonRuleThrowsOnNegativePartitions) {
    EXPECT_THROW(MeteoNumerical::Integration::simpsonRule(square, 0.0, 3.0, -10), std::runtime_error);
}

TEST(IntegrationTest, GaussLegendreQuadrature) {
    // Gauss-Legendre jest dokładne dla wielomianów stopnia 2n-1
    // Dla n=3 punkty, powinno być dokładne dla wielomianu stopnia 5.
    // Nasz wielomian stopnia 2 powinien być obliczony niemal idealnie.
    double result = MeteoNumerical::Integration::GaussLegendre::quadrature(square, 0.0, 3.0, 3);
    EXPECT_NEAR(result, 9.0, 1e-12);
}

TEST(IntegrationTest, GaussLegendreThrowsOnInvalidPointCount) {
    EXPECT_THROW(MeteoNumerical::Integration::GaussLegendre::quadrature(square, 0.0, 3.0, 7), std::out_of_range);
}
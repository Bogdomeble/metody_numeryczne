#include "gtest/gtest.h"
#include "rootfinding.hpp"
#include <cmath>

// Funkcja testowa: f(x) = x^2 - 4, pierwiastki: -2, 2
double parabola(double x) {
    return x * x - 4.0;
}
// Analityczna pochodna: f'(x) = 2x
double parabola_deriv(double x) {
    return 2.0 * x;
}


TEST(RootFindingTest, BisectionMethod) {
    MeteoNumerical::Common::ValueSeries iterations;
    double root = MeteoNumerical::RootFinding::bisection_method(parabola, 0.0, 5.0, 1e-7, 100, iterations);
    EXPECT_NEAR(root, 2.0, 1e-7);
    EXPECT_FALSE(iterations.empty());
}

TEST(RootFindingTest, BisectionMethodFailsOnBadInterval) {
    MeteoNumerical::Common::ValueSeries iterations;
    // f(0)=-4, f(-5)=21. Dobry przedział.
    // f(3)=5, f(5)=21. Zły przedział (ten sam znak).
    double root = MeteoNumerical::RootFinding::bisection_method(parabola, 3.0, 5.0, 1e-7, 100, iterations);
    EXPECT_TRUE(std::isnan(root));
}

TEST(RootFindingTest, NewtonMethodNumeric) {
    MeteoNumerical::Common::ValueSeries iterations;
    double root = MeteoNumerical::RootFinding::newton_method_numeric(parabola, 5.0, 1e-7, 100, iterations);
    EXPECT_NEAR(root, 2.0, 1e-7);
}

TEST(RootFindingTest, NewtonMethodAnalytic) {
    MeteoNumerical::Common::ValueSeries iterations;
    double root = MeteoNumerical::RootFinding::newton_method_analytic(parabola, parabola_deriv, 5.0, 1e-7, 100, iterations);
    EXPECT_NEAR(root, 2.0, 1e-7);
}

TEST(RootFindingTest, NewtonMethodFailsAtZeroDerivative) {
    MeteoNumerical::Common::ValueSeries iterations;
    // Zgadujemy x=0, gdzie pochodna jest 0
    double root = MeteoNumerical::RootFinding::newton_method_analytic(parabola, parabola_deriv, 0.0, 1e-7, 100, iterations);
    EXPECT_TRUE(std::isnan(root));
}

TEST(RootFindingTest, SecantMethod) {
    MeteoNumerical::Common::ValueSeries iterations;
    double root = MeteoNumerical::RootFinding::secant_method(parabola, 4.0, 5.0, 1e-7, 100, iterations);
    EXPECT_NEAR(root, 2.0, 1e-7);
}

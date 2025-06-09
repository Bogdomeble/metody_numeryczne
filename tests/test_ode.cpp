#include "gtest/gtest.h"
#include "ode.hpp"
#include <cmath>

// Prosty problem: y' = y, y(0) = 1. Rozwiązanie analityczne: y(t) = e^t.
double simple_ode_func(double t, double y) {
    return y;
}

TEST(ODETest, RK4Step) {
    // Sprawdzamy pojedynczy krok RK4
    double t0 = 0.0, y0 = 1.0;
    double h = 0.1;
    double y1 = MeteoNumerical::ODE::rk4Step(simple_ode_func, t0, y0, h);
    
    // Analitycznie: y(0.1) = e^0.1 = 1.1051709
    EXPECT_NEAR(y1, exp(0.1), 1e-7); // RK4 jest bardzo dokładny
}

TEST(ODETest, SolveFull) {
    double t0 = 0.0, y0 = 1.0;
    double t_final = 1.0;
    double h = 0.1;
    
    MeteoNumerical::ODE::ODESolution solution = MeteoNumerical::ODE::solve(simple_ode_func, t0, y0, t_final, h, "rk4");
    
    double final_y = solution.back().second;
    // Analitycznie: y(1.0) = e^1 = 2.71828
    EXPECT_NEAR(final_y, exp(1.0), 1e-6);
}

TEST(ODETest, SolveThrowsOnUnknownMethod) {
    EXPECT_THROW(MeteoNumerical::ODE::solve(simple_ode_func, 0.0, 1.0, 1.0, 0.1, "unknown_method"), std::runtime_error);
}

TEST(ODETest, GetSolutionAtTime) {
    MeteoNumerical::ODE::ODESolution solution = {{0.0, 1.0}, {0.1, 1.1}, {0.2, 1.22}};
    
    // Dokładnie w węźle
    EXPECT_NEAR(MeteoNumerical::ODE::getSolutionAtTime(solution, 0.1), 1.1, 1e-9);
    
    // Interpolacja liniowa między węzłami
    EXPECT_NEAR(MeteoNumerical::ODE::getSolutionAtTime(solution, 0.15), 1.16, 1e-9);
}

TEST(ODETest, GetSolutionAtTimeOutOfBoundsIsNaN) {
    MeteoNumerical::ODE::ODESolution solution = {{0.0, 1.0}, {0.1, 1.1}};
    EXPECT_TRUE(std::isnan(MeteoNumerical::ODE::getSolutionAtTime(solution, 0.2)));
}
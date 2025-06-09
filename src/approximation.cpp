#include "approximation.hpp"
#include "linalg.hpp"       // Do rozwiązania układu równań (solveWithLU)
#include <vector>
#include <cmath>
#include <stdexcept>

namespace MeteoNumerical {
namespace Approximation {

Common::ValueSeries polynomialApproximation(
    int degree,
    double a,
    double b,
    const Integration::IntegrableFunction& func_to_approx,
    int integration_partitions) 
{
    int n = degree + 1; // Rozmiar macierzy to (stopień + 1)
    if (n <= 0) {
        throw std::runtime_error("Stopień wielomianu musi być nieujemny.");
    }

    Common::Matrix A(n, Common::ValueSeries(n));
    Common::ValueSeries b_vec(n);

    // Budowanie macierzy A i wektora b
    for (int i = 0; i < n; ++i) {
        // Obliczanie wektora b_vec[i] = całka(f(x) * x^i)
        auto integrand_b = [&](double x) {
            return func_to_approx(x) * std::pow(x, i);
        };
        // Używamy kwadratury Gaussa-Legendre'a z biblioteki (4 węzły, tak jak w Twoim main.cpp)
        b_vec[i] = Integration::GaussLegendre::composite(integrand_b, a, b, 4, integration_partitions);

        // Obliczanie macierzy A[i][j] = całka(x^(i+j))
        for (int j = 0; j < n; ++j) {
            auto integrand_A = [&](double x) {
                return std::pow(x, i + j);
            };
            A[i][j] = Integration::GaussLegendre::composite(integrand_A, a, b, 4, integration_partitions);
        }
    }

    // Rozwiązanie układu równań liniowych A*coeffs = b_vec
    // Używamy stabilnej funkcji solveWithLU z Twojej biblioteki
    Common::ValueSeries coeffs = LinearAlgebra::solveWithLU(A, b_vec);

    return coeffs;
}

} // namespace Approximation
} // namespace MeteoNumerical
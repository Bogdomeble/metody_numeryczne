#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

// Dołączanie modułów biblioteki
#include "common.hpp"
#include "rootfinding.hpp"
#include "integration.hpp"
#include "interpolation.hpp"
#include "meteocalc.hpp"
#include "fileio.hpp"
#include "linalg.hpp" // Dla printVector

// Użycie aliasów dla uproszczenia
using namespace MeteoNumerical;

void showcase_root_finding() {
    std::cout << "\n--- Demonstracja modułu Znajdowania Miejsc Zerowych ---\n";
    // Definiujemy funkcję, której miejsce zerowe chcemy znaleźć: f(x) = x^3 - x - 2
    // Rzeczywiste miejsce zerowe to ok. 1.52138
    auto my_func = [](double x) {
        return x * x * x - x - 2;
    };

    std::cout << "Szukanie miejsca zerowego funkcji f(x) = x^3 - x - 2\n";
    
    Common::ValueSeries iterations;
    double tolerance = 1e-7;
    int max_iter = 100;

    // --- Metoda Bisekcji ---
    try {
        double root_bisection = RootFinding::bisection_method(my_func, 1.0, 2.0, tolerance, max_iter, iterations);
        std::cout << "Metoda Bisekcji:         x = " << root_bisection << " (w " << iterations.size() << " iteracjach)\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Błąd w metodzie bisekcji: " << e.what() << std::endl;
    }

    // --- Metoda Newtona (z pochodną numeryczną) ---
    try {
        double root_newton = RootFinding::newton_method_numeric(my_func, 1.5, tolerance, max_iter, iterations);
        std::cout << "Metoda Newtona (num.):   x = " << root_newton << " (w " << iterations.size() << " iteracjach)\n";

        // Zapiszmy wyniki iteracji metody Newtona do pliku
        std::string filename = "newton_iterations.csv";
        FileIO::saveRootFindingResultsToCSV("Newton", "x^3-x-2", iterations, root_newton);
        std::cout << "Zapisano historię iteracji metody Newtona do pliku: " << filename << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Błąd w metodzie Newtona: " << e.what() << std::endl;
    }
}

void showcase_integration() {
    std::cout << "\n--- Demonstracja modułu Całkowania Numerycznego ---\n";
    // Całkujemy funkcję f(x) = x * sin(x) od 0 do PI.
    // Wynik analityczny to PI (ok. 3.14159265)
    auto func_to_integrate = [](double x) {
        return x * std::sin(x);
    };

    double a = 0.0;
    double b = M_PI;
    int partitions = 100;
    
    std::cout << "Całkowanie f(x) = x*sin(x) od " << a << " do " << b << "\n";
    std::cout << "Wynik analityczny: " << M_PI << "\n";

    double result_simpson = Integration::simpsonRule(func_to_integrate, a, b, partitions);
    std::cout << "Wynik (Reguła Simpsona):   " << result_simpson << "\n";
    
    // Użycie kwadratury Gaussa-Legendre'a (wystarczy kilka punktów dla dużej dokładności)
    double result_gauss = Integration::GaussLegendre::quadrature(func_to_integrate, a, b, 6);
    std::cout << "Wynik (Gauss-Legendre, 6 pkt): " << result_gauss << "\n";
}

void showcase_newton_interpolation() {
    std::cout << "\n--- Demonstracja Interpolacji Wielomianem Newtona ---\n";
    Common::ValueSeries x_nodes = {0.0, 1.0, 3.0, 4.0};
    Common::ValueSeries y_nodes = {1.0, 2.718, 20.08, 54.6}; // f(x) = e^x
    double xp = 2.5;

    std::cout << "Interpolacja f(x)=e^x (te same węzły co w showcase1)\n";

    try {
        // Krok 1: Oblicz tablicę różnic dzielonych
        Common::Matrix div_diff = Interpolation::Newton::calculateDividedDifferences(x_nodes, y_nodes);

        // Krok 2: Wyciągnij współczynniki Newtona
        Common::ValueSeries coeffs = Interpolation::Newton::getNewtonCoefficients(div_diff);
        std::cout << "Współczynniki Newtona: ";
        LinearAlgebra::printVector(coeffs);

        // Krok 3: Wykonaj interpolację
        double yp = Interpolation::Newton::newtonInterpolate(x_nodes, coeffs, xp);
        std::cout << "\nWartość interpolowana w punkcie x = " << xp << " wynosi y = " << yp << "\n";
        std::cout << "(Wynik powinien być identyczny z uzyskanym metodą Lagrange'a)\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Błąd podczas interpolacji Newtona: " << e.what() << std::endl;
    }
}

void showcase_meteo_specifics() {
    std::cout << "\n--- Demonstracja Specyficznych Obliczeń Meteo ---\n";
    double temp_c = 5.0;
    double humidity = 80.0; // %
    double wind_mps = 10.0; // m/s

    try {
        double dew_point = MeteoCalculations::calculate_dew_point(temp_c, humidity);
        std::cout << "Dla T=" << temp_c << "C i Wilgotności=" << humidity << "%, punkt rosy wynosi: " << dew_point << " C\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "Błąd obliczania punktu rosy: " << e.what() << std::endl;
    }

    double wind_chill = MeteoCalculations::calculate_wind_chill(temp_c, wind_mps);
    std::cout << "Dla T=" << temp_c << "C i Wiatru=" << wind_mps << " m/s, temperatura odczuwalna wynosi: " << wind_chill << " C\n";
}


int main() {
    std::cout << "================================================" << std::endl;
    std::cout << "    Program demonstracyjny #2 dla biblioteki" << std::endl;
    std::cout << "             MeteoNumerical" << std::endl;
    std::cout << "================================================" << std::endl;

    showcase_root_finding();
    showcase_integration();
    showcase_newton_interpolation();
    showcase_meteo_specifics();

    std::cout << "\n--- Koniec demonstracji #2 ---\n";
    
    return 0;
}

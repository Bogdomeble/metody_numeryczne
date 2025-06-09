#include <iostream>
#include <vector>
#include <stdexcept>

// Dołączanie potrzebnych modułów z biblioteki
#include "common.hpp"
#include "linalg.hpp"
#include "statistics.hpp"
#include "interpolation.hpp"
#include "fileio.hpp"
#include "ode.hpp"

// Użycie aliasów dla uproszczenia
using namespace MeteoNumerical;

void showcase_linear_algebra() {
    std::cout << "\n--- Demonstracja modułu Algebry Liniowej ---\n";
    Common::Matrix A = {{4, 1, -1}, {2, 5, 1}, {1, 1, 3}};
    Common::ValueSeries b = {8, 17, 10}; // Oczekiwane rozwiązanie: x = [1, 3, 2]

    try {
        std::cout << "Macierz A:" << std::endl;
        LinearAlgebra::printMatrix(A);
        std::cout << "Wektor b:" << std::endl;
        LinearAlgebra::printVector(b);

        Common::ValueSeries x = LinearAlgebra::solveWithLU(A, b);

        std::cout << "\nRozwiazanie ukladu Ax=b:" << std::endl;
        LinearAlgebra::printVector(x);
    } catch (const std::runtime_error& e) {
        std::cerr << "Błąd podczas rozwiązywania układu równań: " << e.what() << std::endl;
    }
}

void showcase_statistics() {
    std::cout << "\n--- Demonstracja modułu Statystyki ---\n";
    Common::ValueSeries temps = {12.5, 13.1, 11.9, 12.8, 13.5, 14.0, 12.2};
    std::cout << "Przykładowe temperatury [C]: ";
    LinearAlgebra::printVector(temps, std::cout, 2);

    double avg = Statistics::calculate_average(temps);
    double std_dev = Statistics::calculate_std_dev(temps);
    double min_val = Statistics::find_min(temps);
    double max_val = Statistics::find_max(temps);

    std::cout << "Średnia: " << avg << " C\n";
    std::cout << "Odchylenie standardowe: " << std_dev << " C\n";
    std::cout << "Min: " << min_val << " C, Max: " << max_val << " C\n";
}

void showcase_interpolation_and_fileio() {
    std::cout << "\n--- Demonstracja modułu Interpolacji i I/O ---\n";
    Common::ValueSeries x_nodes = {0.0, 1.0, 3.0, 4.0};
    Common::ValueSeries y_nodes = {1.0, 2.718, 20.08, 54.6}; // f(x) = e^x
    double xp = 2.5;

    std::cout << "Interpolacja funkcji f(x) = e^x na podstawie węzłów:\n";
    std::cout << "x = "; LinearAlgebra::printVector(x_nodes);
    std::cout << "y = "; LinearAlgebra::printVector(y_nodes);
    
    double yp = Interpolation::Lagrange::lagrangeInterpolate(x_nodes, y_nodes, xp);
    
    std::cout << "\nWartość interpolowana w punkcie x = " << xp << " wynosi y = " << yp << "\n";
    std::cout << "(Prawdziwa wartość e^2.5 to ok. 12.18)\n";

    // Wygeneruj gęstą siatkę punktów i zapisz do CSV
    Common::ValueSeries x_dense, y_dense;
    for (double x = 0.0; x <= 4.0; x += 0.1) {
        x_dense.push_back(x);
        y_dense.push_back(Interpolation::Lagrange::lagrangeInterpolate(x_nodes, y_nodes, x));
    }

    try {
        std::string filename = "interpolation_output.csv";
        FileIO::writeXYDataToCSV(filename, x_dense, y_dense, "x", "interpolated_y");
        std::cout << "\nWyniki interpolacji zostały zapisane do pliku: " << filename << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Błąd zapisu do pliku: " << e.what() << std::endl;
    }
}

void showcase_ode_solver() {
    std::cout << "\n--- Demonstracja modułu Równań Różniczkowych ---\n";
    // Równanie: y'(t) = -2*t*y, y(0) = 1. Rozwiązanie analityczne: y(t) = e^(-t^2)
    auto my_ode_func = [](double t, double y) {
        return -2.0 * t * y;
    };

    double t0 = 0.0, y0 = 1.0, t_final = 2.0, h = 0.1;

    std::cout << "Rozwiązywanie y'(t) = -2*t*y, z warunkiem początkowym y(" << t0 << ") = " << y0 << "\n";
    
    ODE::ODESolution solution = ODE::solve(my_ode_func, t0, y0, t_final, h, "rk4");

    std::cout << "\nWyniki uzyskane metodą RK4 co " << h << "s:\n";
    std::cout << "t       |   y_numeryczne  |  y_analityczne\n";
    std::cout << "------------------------------------------\n";
    for (const auto& point : solution) {
        printf("%-7.2f | %-15.8f | %-15.8f\n", point.first, point.second, exp(-point.first * point.first));
    }
}


int main() {
    std::cout << "================================================" << std::endl;
    std::cout << "    Program demonstracyjny dla biblioteki" << std::endl;
    std::cout << "             MeteoNumerical" << std::endl;
    std::cout << "================================================" << std::endl;

    showcase_linear_algebra();
    showcase_statistics();
    showcase_interpolation_and_fileio();
    showcase_ode_solver();

    std::cout << "\n--- Koniec demonstracji ---\n";

    return 0;
}

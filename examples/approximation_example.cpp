#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

// Nagłówki z naszej biblioteki
#include "approximation.hpp"
#include "common.hpp"

// Funkcja, którą będziemy aproksymować (taka sama jak w oryginalnym pliku)
double f(double x) {
    return exp(x) * cos(6.0 * x) - x * x * x + 5.0 * x * x - 10.0;
}

int main() {
    // Przedział aproksymacji
    constexpr double a = 1.5, b = 4.5;
    // Stopnie wielomianów do przetestowania
    std::vector<int> degrees = {10, 20, 30};
    // Liczba podprzedziałów do całkowania (zgodnie z oryginałem)
    constexpr int partitions_for_integration = 93;

    for (int degree : degrees) {
        std::cout << "\n=======================================================\n";
        std::cout << "Aproksymacja wielomianem stopnia " << degree << ":\n";
        std::cout << "=======================================================\n";

        try {
            // JEDNO wywołanie funkcji z biblioteki, która robi całą magię!
            MeteoNumerical::Common::ValueSeries coeffs = 
                MeteoNumerical::Approximation::polynomialApproximation(
                    degree, a, b, f, partitions_for_integration
                );

            // Wyświetlanie wyników
            std::cout << "Obliczone współczynniki wielomianu:\n";
            for (size_t i = 0; i < coeffs.size(); ++i) {
                std::cout << "a_" << i << " = " << std::scientific << std::setprecision(8) << coeffs[i] << std::endl;
            }

            // Porównanie wyników i obliczenie błędu
            std::cout << "\nPorównanie wartości na " << partitions_for_integration << " punktach:\n";
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "x\t\tf(x)\t\tW(x)\t\t|f(x)-W(x)|\n";

            double max_error = 0.0;
            double sum_error_squared = 0.0;
            int points_to_check = 31; // Liczba punktów do ewaluacji błędu

            for (int i = 0; i < points_to_check; ++i) {
                double x = a + i * (b - a) / (points_to_check - 1);
                double fx_val = f(x);
                // Używamy funkcji Hornera z biblioteki
                double Wx_val = MeteoNumerical::Common::evaluatePolynomialHorner(coeffs, x);
                double error = std::abs(fx_val - Wx_val);

                std::cout << x << "\t" << fx_val << "\t" << Wx_val << "\t" << error << std::endl;

                max_error = std::max(max_error, error);
                sum_error_squared += error * error;
            }

            double rms_error = sqrt(sum_error_squared / points_to_check);
            std::cout << "\nBłąd maksymalny: " << max_error << std::endl;
            std::cout << "Błąd średniokwadratowy (RMS): " << rms_error << std::endl;

        } catch (const std::runtime_error& e) {
            std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
        }
    }

    return 0;
}
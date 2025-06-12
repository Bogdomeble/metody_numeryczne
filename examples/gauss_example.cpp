// Nazwa pliku: examples/test_gauss.cpp

#include <iostream>
#include <vector>
#include <stdexcept>
#include "common.hpp"
#include "linalg.hpp"

void solve_and_print(const std::string& task_name, MeteoNumerical::Common::Matrix A, MeteoNumerical::Common::ValueSeries b) {
    std::cout << "\n--- Zadanie " << task_name << " ---\n";

    try {
        MeteoNumerical::Common::ValueSeries x = MeteoNumerical::LinearAlgebra::gaussElimination(A, b);

        std::cout << "Rozwiazanie (wektor x):\n";
        MeteoNumerical::LinearAlgebra::printVector(x);

    } catch (const std::runtime_error& e) {
        std::cerr << "Blad podczas rozwiazywania ukladu: " << e.what() << std::endl;
    }
}

int main() {
    // --- Dane dla zadania A ---
    
    MeteoNumerical::Common::Matrix A_A = {
        {12, -14, -6,  8,  -4},
        {10, -14, 16, 15,  13},
        {16,  15,  6, 19,   5},
        {11, -12, -6, -2,  18},
        {13,  13, -1, -12, -16}
    };
    MeteoNumerical::Common::ValueSeries b_A = {3, 3, -7, 4, 0};
    solve_and_print("A", A_A, b_A);


    // --- Dane da zadania B ---

    MeteoNumerical::Common::Matrix A_B = {
        {16, -11, -1,  -6,  -1},
        {12,   5,  0, -19, -10},
        {-11,  -5,  1, -13, -11},
        {16, -11, -1,  -6,  -1}, // Ten wiersz jest identyczny jak pierwszy
        {-8,  15, -2,  17, -11}
    };
    MeteoNumerical::Common::ValueSeries b_B = {-3, -3, 17, 10, -13};
    solve_and_print("B", A_B, b_B);


    // --- Dane dla zadania C ---
    MeteoNumerical::Common::Matrix A_C = {
        {0,  -17, -16,  -1, -15},
        {-20,   0, -15,   6, -11},
        {17,  -8,   0,   4,   8},
        {-9,  17, -12,   0,   0},
        {-14,  11, -16, -17,   0}
    };
    MeteoNumerical::Common::ValueSeries b_C = {-2, 9, 2, 12, -14};
    solve_and_print("C", A_C, b_C);

    return 0;
}
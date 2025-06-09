#include "gtest/gtest.h"
#include "statistics.hpp"
#include <vector>
#include <cmath> // Dla std::isnan

using namespace MeteoNumerical;

// Test Suite dla modułu Statistics
TEST(StatisticsTest, AverageCalculationCorrect) {
    // Przypadek poprawny: Obliczenie średniej dla typowego wektora.
    Common::ValueSeries data = {10.0, 15.0, 20.0, 25.0, 30.0};
    EXPECT_NEAR(Statistics::calculate_average(data), 20.0, 1e-9);
}

TEST(StatisticsTest, AverageForEmptyVectorIsNaN) {
    // Przypadek błędny/brzegowy: Pusty wektor powinien zwrócić NaN.
    Common::ValueSeries data = {};
    EXPECT_TRUE(std::isnan(Statistics::calculate_average(data)));
}

TEST(StatisticsTest, StdDevCalculationCorrect) {
    // Przypadek poprawny: Obliczenie odchylenia standardowego.
    Common::ValueSeries data = {2, 4, 4, 4, 5, 5, 7, 9};
    
    // POPRAWKA: Wartość 2.0 była niepoprawna. Prawidłowy wynik to ~2.138.
    double expected_std_dev = 2.138089935299; 
    EXPECT_NEAR(Statistics::calculate_std_dev(data, false), expected_std_dev, 1e-9);
}
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


// TEST BŁĘDNY: dla odchylenia standardowego
TEST(StatisticsTest, StdDevWithLessThanTwoElementsIsNaN) {
    MeteoNumerical::Common::ValueSeries values = {10.0};
    EXPECT_TRUE(std::isnan(MeteoNumerical::Statistics::calculate_std_dev(values)));
}

TEST(StatisticsTest, FindMin) {
    MeteoNumerical::Common::ValueSeries values = {10.0, -5.0, 30.0, 0.0};
    EXPECT_NEAR(MeteoNumerical::Statistics::find_min(values), -5.0, 1e-9);
}

TEST(StatisticsTest, FindMinEmptyIsNaN) {
    MeteoNumerical::Common::ValueSeries values = {};
    EXPECT_TRUE(std::isnan(MeteoNumerical::Statistics::find_min(values)));
}

TEST(StatisticsTest, FindMax) {
    MeteoNumerical::Common::ValueSeries values = {10.0, -5.0, 30.0, 0.0};
    EXPECT_NEAR(MeteoNumerical::Statistics::find_max(values), 30.0, 1e-9);
}

TEST(StatisticsTest, FindMaxEmptyIsNaN) {
    MeteoNumerical::Common::ValueSeries values = {};
    EXPECT_TRUE(std::isnan(MeteoNumerical::Statistics::find_max(values)));
}

TEST(StatisticsTest, CalculateVariance) {
    MeteoNumerical::Common::ValueSeries values = {2, 4, 4, 4, 5, 5, 7, 9};
    double std_dev = 2.138089935;
    EXPECT_NEAR(MeteoNumerical::Statistics::calculate_variance(values, false), std_dev * std_dev, 1e-9);
}

TEST(StatisticsTest, CalculateVarianceWithLessThanTwoElementsIsNaN) {
    MeteoNumerical::Common::ValueSeries values = {1.0};
    EXPECT_TRUE(std::isnan(MeteoNumerical::Statistics::calculate_variance(values)));
}

TEST(StatisticsTest, CalculateMSE) {
    MeteoNumerical::Common::ValueSeries y_true = {1.0, 2.0, 3.0, 4.0};
    MeteoNumerical::Common::ValueSeries y_pred = {1.5, 2.5, 3.5, 4.5};
    // (0.5^2 + 0.5^2 + 0.5^2 + 0.5^2) / 4 = (0.25 * 4) / 4 = 0.25
    EXPECT_NEAR(MeteoNumerical::Statistics::calculate_mse(y_true, y_pred), 0.25, 1e-9);
}

TEST(StatisticsTest, CalculateMSEWithMismatchedSizesIsNaN) {
    MeteoNumerical::Common::ValueSeries y_true = {1.0, 2.0, 3.0};
    MeteoNumerical::Common::ValueSeries y_pred = {1.5, 2.5};
    EXPECT_TRUE(std::isnan(MeteoNumerical::Statistics::calculate_mse(y_true, y_pred)));
}
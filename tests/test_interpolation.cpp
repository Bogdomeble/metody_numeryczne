#include "gtest/gtest.h"
#include "interpolation.hpp"
#include <stdexcept>

// TEST POPRAWNY: Interpolacja w węźle powinna zwrócić wartość z tego węzła
TEST(InterpolationTest, LagrangeReturnsNodeValueAtNode) {
    MeteoNumerical::Common::ValueSeries x = {0.0, 1.0, 2.0};
    MeteoNumerical::Common::ValueSeries y = {1.0, 3.0, 2.0};
    
    // W punkcie x=1.0 interpolacja powinna dać dokładnie y=3.0
    double result = MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, 1.0);
    EXPECT_NEAR(result, 3.0, 1e-12);
}

// TEST BŁĘDNY: Zduplikowane węzły x
TEST(InterpolationTest, LagrangeThrowsOnDuplicateXNodes) {
    MeteoNumerical::Common::ValueSeries x = {0.0, 1.0, 1.0}; // Zduplikowany węzeł
    MeteoNumerical::Common::ValueSeries y = {1.0, 3.0, 2.0};

    EXPECT_THROW(MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, 1.5), std::runtime_error);
}


// --- Testy walidacji danych wejściowych ---

TEST(InterpolationInputValidation, ThrowsOnEmptyVectors) {
    MeteoNumerical::Common::ValueSeries empty_x, empty_y;
    MeteoNumerical::Common::ValueSeries newton_coeffs;

    EXPECT_THROW(MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(empty_x, empty_y, 1.0), std::runtime_error);
    EXPECT_THROW(MeteoNumerical::Interpolation::Newton::calculateDividedDifferences(empty_x, empty_y), std::runtime_error);
    EXPECT_THROW(MeteoNumerical::Interpolation::Newton::newtonInterpolate(empty_x, newton_coeffs, 1.0), std::runtime_error);
}

// --- Testy przypadków brzegowych ---

TEST(InterpolationEdgeCases, HandlesSingleNodeInterpolation) {
    MeteoNumerical::Common::ValueSeries x = {5.0};
    MeteoNumerical::Common::ValueSeries y = {10.0};
    double target_x = 123.45; // dowolny punkt

    // Dla jednego węzła, interpolacja powinna zawsze zwrócić wartość tego węzła
    double lagrange_result = MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, target_x);
    EXPECT_NEAR(lagrange_result, 10.0, 1e-9);

    // Test dla Newtona
    auto div_diff = MeteoNumerical::Interpolation::Newton::calculateDividedDifferences(x, y);
    auto coeffs = MeteoNumerical::Interpolation::Newton::getNewtonCoefficients(div_diff);
    double newton_result = MeteoNumerical::Interpolation::Newton::newtonInterpolate(x, coeffs, target_x);
    
    ASSERT_EQ(coeffs.size(), 1);
    EXPECT_NEAR(coeffs[0], 10.0, 1e-9);
    EXPECT_NEAR(newton_result, 10.0, 1e-9);
}

// --- Testy właściwości matematycznych ---

TEST(InterpolationMathematicalProperties, ReproducesConstantFunctionExactly) {
    MeteoNumerical::Common::ValueSeries x = {0.0, 1.0, 2.0, 3.0};
    MeteoNumerical::Common::ValueSeries y = {7.0, 7.0, 7.0, 7.0}; // f(x) = 7
    
    // Interpolacja funkcji stałej powinna dać wartość tej stałej w dowolnym punkcie
    EXPECT_NEAR(MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, 1.5), 7.0, 1e-9);
    EXPECT_NEAR(MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, -10.0), 7.0, 1e-9);
}

TEST(InterpolationMathematicalProperties, ReproducesLinearFunctionExactly) {
    // f(x) = 2x + 3
    MeteoNumerical::Common::ValueSeries x = {0.0, 2.0, 5.0, 10.0};
    MeteoNumerical::Common::ValueSeries y = {3.0, 7.0, 13.0, 23.0};
    
    // Interpolacja wielomianem stopnia n funkcji, która jest wielomianem stopnia <= n,
    // musi dać wynik dokładny. Testujemy to dla funkcji liniowej.
    
    // Punkt pomiędzy węzłami
    double target_x1 = 1.0;
    double expected_y1 = 2.0 * target_x1 + 3.0; // 5.0
    EXPECT_NEAR(MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, target_x1), expected_y1, 1e-9);

    // Punkt poza węzłami (ekstrapolacja)
    double target_x2 = -2.0;
    double expected_y2 = 2.0 * target_x2 + 3.0; // -1.0
    EXPECT_NEAR(MeteoNumerical::Interpolation::Lagrange::lagrangeInterpolate(x, y, target_x2), expected_y2, 1e-9);
}


// --- Testy funkcji pomocniczych ---

TEST(HelperFunctionTest, SelectNodesByStep) {
    unsigned int k = 2; // Krok do wyboru węzłów
    MeteoNumerical::Common::ValueSeries x_all = {0, 1, 2, 3, 4, 5, 6, 7};
    MeteoNumerical::Common::ValueSeries y_all = {0, 10, 20, 30, 40, 50, 60, 70};
    MeteoNumerical::Common::ValueSeries x_nodes, y_nodes;

    // Test 1: Wybieranie co drugi węzeł (k=2)
    MeteoNumerical::Interpolation::selectNodesByStep(x_all, y_all, x_nodes, y_nodes, k);
    MeteoNumerical::Common::ValueSeries expected_x1 = {0, 2, 4, 6};
    MeteoNumerical::Common::ValueSeries expected_y1 = {0, 20, 40, 60};
    EXPECT_EQ(x_nodes, expected_x1);
    EXPECT_EQ(y_nodes, expected_y1);
    k = 3;
    // Test 2: Wybieranie co trzeci węzeł (k=3)
    MeteoNumerical::Interpolation::selectNodesByStep(x_all, y_all, x_nodes, y_nodes, k);
    MeteoNumerical::Common::ValueSeries expected_x2 = {0, 3, 6};
    MeteoNumerical::Common::ValueSeries expected_y2 = {0, 30, 60};
    EXPECT_EQ(x_nodes, expected_x2);
    EXPECT_EQ(y_nodes, expected_y2);
    k = 1;
    // Test 3: Wybieranie co pierwszy węzeł (k=1) powinno dać kopię oryginału
    MeteoNumerical::Interpolation::selectNodesByStep(x_all, y_all, x_nodes, y_nodes, k);
    EXPECT_EQ(x_nodes, x_all);
    EXPECT_EQ(y_nodes, y_all);

    k = 0;
    // Test 4: Sprawdzenie rzucania wyjątku dla k=0
    EXPECT_THROW(MeteoNumerical::Interpolation::selectNodesByStep(x_all, y_all, x_nodes, y_nodes, 0), std::runtime_error);
}
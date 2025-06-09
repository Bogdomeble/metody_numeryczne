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
#ifndef METEO_INTERPOLATION_HPP
#define METEO_INTERPOLATION_HPP

#include "common.hpp"
#include <functional>

namespace MeteoNumerical {
namespace Interpolation {

    namespace Newton {
        Common::Matrix calculateDividedDifferences(const Common::ValueSeries& x, const Common::ValueSeries& y);
        Common::ValueSeries getNewtonCoefficients(const Common::Matrix& divDiff);
        double newtonInterpolate(const Common::ValueSeries& x_nodes, const Common::ValueSeries& newton_coeffs, double xp);
    } // namespace Newton

    namespace Lagrange {
        double lagrangeInterpolate(const Common::ValueSeries& x_nodes, const Common::ValueSeries& y_nodes, double xp);
    } // namespace Lagrange

    void selectNodesByStep(const Common::ValueSeries& x_all, const Common::ValueSeries& y_all,
                                  Common::ValueSeries& x_nodes, Common::ValueSeries& y_nodes, unsigned int k_step);

    using InterpolatorFunc = std::function<double(const Common::ValueSeries&, const Common::ValueSeries&, double)>;

    double calculateInterpolationMSE(
            const Common::ValueSeries& x_all, const Common::ValueSeries& y_all,
            const Common::ValueSeries& x_nodes, const Common::ValueSeries& y_nodes_or_coeffs,
            InterpolatorFunc interpolator,
            Common::ValueSeries* out_mse_vector = nullptr,
            Common::ValueSeries* out_x_error_points = nullptr);

} // namespace Interpolation
} // namespace MeteoNumerical

#endif // METEO_INTERPOLATION_HPP
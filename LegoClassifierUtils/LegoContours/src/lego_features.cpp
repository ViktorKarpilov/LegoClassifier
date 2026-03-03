#include <lego_features.hpp>
#include <lego_contours.hpp>

using namespace features;

lego_features features::extract_features(const string_view &image_path)
{
    const auto contour = contours::lego_contour(image_path);
    const auto params = contour.calculate_contour_parameters();

    return {
        .area = params.area,
        .perimeter = params.perimeter,
        .aspect_ratio = params.aspect_ration,
        .circles = contour.get_circle_count()
    };
}

#include <lego_contours.hpp>
#include "catch2/catch_test_macros.hpp"
TEST_CASE("Contour created from file", "[contour]")
{
    const auto contour = contours::lego_contour("./static/image.jpg");
    REQUIRE(contour.has_value());
}

TEST_CASE("Contour created is contour", "[contour]")
{
    const auto contour = contours::lego_contour("./static/image.jpg");
    REQUIRE(contour.build_contours_image().rows != 0);
}

TEST_CASE("Contour area decrease after smoothing", "[contour]")
{
    auto contour = contours::lego_contour("./static/image.jpg");
    REQUIRE(contour.has_value());
    const auto primal_area = contour.calculate_contour_parameters();
    contour.approximate_contour_area(30);
    const auto secondary_area = contour.calculate_contour_parameters();
    REQUIRE(secondary_area.area < primal_area.area);
    REQUIRE(secondary_area.area > 0);
    REQUIRE(primal_area.area > 0);
}

TEST_CASE("Perimeter decrease after smoothing", "[contour]")
{
    auto contour = contours::lego_contour("./static/image.jpg");
    REQUIRE(contour.has_value());
    const auto primal_area = contour.calculate_contour_parameters();
    contour.approximate_contour_area(30);
    const auto secondary_area = contour.calculate_contour_parameters();
    REQUIRE(secondary_area.perimeter < primal_area.perimeter);
    REQUIRE(secondary_area.perimeter > 0);
    REQUIRE(primal_area.perimeter > 0);
}

TEST_CASE("Aspect ration calculates", "[contour]")
{
    const auto contour = contours::lego_contour("./static/image.jpg");
    REQUIRE(contour.has_value());
    const auto aspect = contour.calculate_contour_parameters();

    REQUIRE(aspect.aspect_ration > 0);
    REQUIRE(aspect.aspect_ration < 1);
}

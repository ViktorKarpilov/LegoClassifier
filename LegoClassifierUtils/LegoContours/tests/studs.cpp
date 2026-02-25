#include <lego_contours.hpp>
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Two by four has eight stubs", "[contour]")
{
    const auto contour = contours::lego_contour("./static/two_by_four_with_holes.jpg");
    REQUIRE(contour.has_value());
    REQUIRE(contour.get_circle_count() == 8);
}

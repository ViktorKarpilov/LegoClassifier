#include <lego_contours.hpp>
#include <lego_features.hpp>
#include <opencv2/highgui.hpp>
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Overlay image shows contours and studs", "[visual]")
{
    const auto contour = contours::lego_contour("./static/received.png");
    REQUIRE(contour.has_value());

    const auto overlay = contour.build_overlay_image();
    REQUIRE(!overlay.empty());

    const string window_name = "Contour + Studs Overlay";
    imshow(window_name, overlay);
    moveWindow(window_name, 100, 100);
    waitKey(0);
}

TEST_CASE("Lego features should be correct", "[features]")
{
    const auto features = features::extract_features("./static/two_by_four_with_holes.png");

    REQUIRE(features.circles == 8);

    REQUIRE(features.area > 264000);
    REQUIRE(features.area < 300000);

    REQUIRE(features.perimeter > 2000);
    REQUIRE(features.perimeter < 2500);

    REQUIRE(features.aspect_ratio < 0.6);
    REQUIRE(features.aspect_ratio > 0.4);
}
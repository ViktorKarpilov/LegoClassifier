#pragma once
#include <string>
#include <opencv2/core/mat.hpp>

using namespace cv;
using namespace std;

namespace contours
{
    struct contour_parameters
    {
        double area;
        double perimeter;
        double aspect_ration;
    };

    class lego_contour
    {
    public:
        explicit lego_contour(const string_view& image_path);

        [[nodiscard]] bool has_value() const;
        [[nodiscard]] Mat build_contours_image(double epsilon = 3) const;
        Mat draw_contour(int index) const;
        [[nodiscard]] contour_parameters calculate_contour_parameters() const;

        /**
         * WARNING! Not const - it's going to change contours !
         */
        void approximate_contour_area(double epsilon) noexcept;

    private:
        vector<vector<Point>> contours_;
        vector<Vec4i> hierarchy_;
        int32_t height_;
        int32_t width_;
    };
}

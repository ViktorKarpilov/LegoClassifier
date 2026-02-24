#include <lego_contours.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

contours::lego_contour::lego_contour(const string_view& image_path)
{
    const auto image = imread(String{image_path}, IMREAD_GRAYSCALE);
    width_ = image.cols;
    height_ = image.rows;

    Mat binary;
    threshold(image, binary, 100, 255, THRESH_BINARY_INV);

    findContours(binary, contours_, hierarchy_, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
}

bool contours::lego_contour::has_value() const
{
    return !contours_.empty();
}

Mat contours::lego_contour::build_contours_image(const double epsilon) const
{
    auto contours = vector<vector<Point>>(contours_.size());

    for( size_t k = 0; k < contours_.size(); k++ )
        approxPolyDP(Mat(contours_[k]), contours[k], epsilon, true);

    Mat cnt_img = Mat::zeros(height_, width_, CV_8UC3);
    drawContours( cnt_img, contours,  -1, Scalar(128,255,255),
                  1, LINE_AA, hierarchy_, 0);
    return cnt_img;
}

Mat contours::lego_contour::draw_contour(const int index) const
{
    const auto contours = vector({contours_[index]});

    Mat cnt_img = Mat::zeros(height_, width_, CV_8UC3);
    drawContours( cnt_img, contours,  -1, Scalar(128,255,255),
                  1, LINE_AA, hierarchy_, 0);
    return cnt_img;
}

contours::contour_parameters contours::lego_contour::calculate_contour_parameters() const
{
    assert(this->has_value());

    contour_parameters parameters = {};
    int index = 0;
    for (int i = 0; i < contours_.size(); i++)
        if (hierarchy_[i][3] == -1)
            if (const auto area = contourArea(contours_[i]); area > parameters.area)
            {
                parameters.area = area;
                index = i;
            }

    parameters.perimeter = arcLength(contours_[index], true);
    const auto box = boundingRect(contours_[index]);
    parameters.aspect_ration = static_cast<double>(box.height)/box.width;

    return parameters;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void contours::lego_contour::approximate_contour_area(const double epsilon) noexcept
{
    for (auto &contour : contours_)
    {
        vector<Point> buffer;
        approxPolyDP(contour, buffer, epsilon, true);
        contour = std::move(buffer);
    }
}
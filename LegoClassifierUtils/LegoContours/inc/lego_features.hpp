#pragma once
#include <string>

using namespace std;

namespace features
{
    struct lego_features
    {
        // Size of piece
        double area;

        // Find complex shape
        double perimeter;

        // Different L from I shape
        double aspect_ratio;

        // Number of studs
        int circles;
    };

    lego_features extract_features(const string_view &image_path);
}

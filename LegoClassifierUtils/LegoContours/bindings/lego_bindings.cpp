#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <opencv2/core/mat.hpp>

#include "lego_features.hpp"
#include "lego_contours.hpp"

namespace py = pybind11;

py::array_t<uint8_t> mat_to_numpy(const cv::Mat& mat) {
    if (mat.empty()) {
        return py::array_t<uint8_t>();
    }

    std::vector<py::ssize_t> shape;
    std::vector<py::ssize_t> strides;

    if (mat.channels() == 1) {
        shape = {mat.rows, mat.cols};
        strides = {static_cast<py::ssize_t>(mat.step[0]),
                   static_cast<py::ssize_t>(mat.elemSize1())};
    } else {
        shape = {mat.rows, mat.cols, mat.channels()};
        strides = {static_cast<py::ssize_t>(mat.step[0]),
                   static_cast<py::ssize_t>(mat.step[1]),
                   static_cast<py::ssize_t>(mat.elemSize1())};
    }

    cv::Mat* mat_copy = new cv::Mat(mat.clone());

    py::capsule free_when_done(mat_copy, [](void* ptr) {
        delete static_cast<cv::Mat*>(ptr);
    });

    return py::array_t<uint8_t>(
        shape,
        strides,
        mat_copy->data,
        free_when_done
    );
}

PYBIND11_MODULE(lego_classifier, m) {
    m.doc() = "LEGO Classifier Python bindings";

    auto features_module = m.def_submodule("features", "Feature extraction module");

    py::class_<features::lego_features>(features_module, "lego_features")
        .def(py::init<>())
        .def_readwrite("area", &features::lego_features::area)
        .def_readwrite("perimeter", &features::lego_features::perimeter)
        .def_readwrite("aspect_ratio", &features::lego_features::aspect_ratio)
        .def_readwrite("circles", &features::lego_features::circles)
        .def("__repr__", [](const features::lego_features& f) {
            return "lego_features(area=" + std::to_string(f.area) +
                   ", perimeter=" + std::to_string(f.perimeter) +
                   ", aspect_ratio=" + std::to_string(f.aspect_ratio) +
                   ", circles=" + std::to_string(f.circles) + ")";
        });

    features_module.def("extract_features", &features::extract_features,
        py::arg("image_path"),
        "Extract LEGO features from an image file");

    auto contours_module = m.def_submodule("contours", "Contour processing module");

    py::class_<contours::contour_parameters>(contours_module, "contour_parameters")
        .def(py::init<>())
        .def_readwrite("area", &contours::contour_parameters::area)
        .def_readwrite("perimeter", &contours::contour_parameters::perimeter)
        .def_readwrite("aspect_ratio", &contours::contour_parameters::aspect_ration)
        .def("__repr__", [](const contours::contour_parameters& p) {
            return "contour_parameters(area=" + std::to_string(p.area) +
                   ", perimeter=" + std::to_string(p.perimeter) +
                   ", aspect_ratio=" + std::to_string(p.aspect_ration) + ")";
        });

    py::class_<contours::lego_contour>(contours_module, "lego_contour")
        .def(py::init<const std::string_view&>(), py::arg("image_path"))
        .def("has_value", &contours::lego_contour::has_value,
            "Check if the contour data is valid")
        .def("build_contours_image", [](const contours::lego_contour& self, double epsilon) {
            return mat_to_numpy(self.build_contours_image(epsilon));
        }, py::arg("epsilon") = 3.0,
            "Build an image showing the detected contours")
        .def("build_overlay_image", [](const contours::lego_contour& self) {
            return mat_to_numpy(self.build_overlay_image());
        }, "Build an overlay image with contours on the original")
        .def("draw_contour", [](const contours::lego_contour& self, int index) {
            return mat_to_numpy(self.draw_contour(index));
        }, py::arg("index"),
            "Draw a specific contour by index")
        .def("calculate_contour_parameters", &contours::lego_contour::calculate_contour_parameters,
            "Calculate contour parameters (area, perimeter, aspect_ratio)")
        .def("get_circle_count", &contours::lego_contour::get_circle_count,
            "Get the number of detected circles (studs)")
        .def("approximate_contour_area", &contours::lego_contour::approximate_contour_area,
            py::arg("epsilon"),
            "Approximate contour area with given epsilon value");
}

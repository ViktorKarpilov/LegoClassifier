set(LIB_SRCS
        lego_contour.cpp
        lego_features.cpp
)

add_library(${Library} STATIC ${LIB_SRCS})
target_link_libraries(${Library} PUBLIC opencv_core opencv_imgcodecs opencv_imgproc opencv_highgui)

set(PLATFORM_LIB PLATFORM_LIB)
set(PLATFORM_LIB ${PLATFORM_LIB} PARENT_SCOPE)

set(INC_DIRECTORIES
        Inc/Camera
)

set(OV2640_HEADERS_DIRS
        Camera/OV2640
)

set(OV2640_SRCS
        Camera/OV2640/ov2640.c
        Camera/OV2640/ov2640_regs.c
)

set(CAMERA_SRCS
        Camera/camera.cpp
)

add_library(${PLATFORM_LIB}
        ${OV2640_SRCS}
        ${CAMERA_SRCS}
)

target_include_directories(${PLATFORM_LIB} PUBLIC ${INC_DIRECTORIES})
target_include_directories(${PLATFORM_LIB} PRIVATE ${OV2640_HEADERS_DIRS})

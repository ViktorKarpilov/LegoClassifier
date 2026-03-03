find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)

pybind11_add_module(lego_classifier lego_bindings.cpp)

target_compile_definitions(lego_classifier
        PRIVATE VERSION_INFO="1.0.0")

target_link_libraries(lego_classifier PRIVATE
    ${Library}
    Python3::NumPy
)

target_include_directories(lego_classifier PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
)

# Place output in bindings folder directly (avoid Release/Debug subdirs on Windows)
set_target_properties(lego_classifier PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
)

cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(TESTING_BUILD "Enable testing build" OFF)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME LegoClassifier)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_MODULE_PATH cmake)

set(BUSINESS_LOGIC_LIB BUSINESS_LOGIC)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

#message(${CMAKE_MODULE_PATH})
include(${CMAKE_MODULE_PATH}/CPM.cmake)

if(TESTING_BUILD)
    message("Testing build")

    enable_language(CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_TOOLCHAIN_FILE "")

    CPMAddPackage("gh:catchorg/Catch2@3.12.0")
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(CTest)
    enable_testing()

    add_subdirectory(BusinessLogic)
    add_subdirectory(Tests)
else ()
    message("Non test build")

    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME})

    # Add STM32CubeMX generated sources
    add_subdirectory(cmake/stm32cubemx)
    add_subdirectory(Platform)
    add_subdirectory(BusinessLogic)
    add_subdirectory(App)

    # Link directories setup
    target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
            # Add user defined library search paths
    )

    # Add sources to executable
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
            # Add user sources here
    )

    # Add include paths
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
            # Add user defined include paths
    )

    # Add project symbols (macros)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
            # Add user defined symbols
    )

    # Remove wrong libob.a library dependency when using cpp files
    list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

    # Add linked libraries
    target_link_libraries(${CMAKE_PROJECT_NAME}
            stm32cubemx
            ${APP_LIB}
            # Add user defined libraries
    )
endif ()
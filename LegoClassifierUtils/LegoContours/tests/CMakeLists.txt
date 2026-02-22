if(ENABLE_TESTING)
    include(Catch)

    set(TESTS_FILES
            main.cpp)

    add_executable(TESTS ${TESTS_FILES})
    target_link_libraries(TESTS PUBLIC
            Library
            Catch2::Catch2WithMain
    )

    catch_discover_tests(TESTS)
endif()
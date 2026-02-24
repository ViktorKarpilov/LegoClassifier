if(ENABLE_TESTING)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    message(${catch2_SOURCE_DIR})
    include(CTest)
    enable_testing()

    include(Catch)

    set(TESTS_FILES
            main.cpp)

    add_executable(TESTS ${TESTS_FILES})
    target_link_libraries(TESTS PUBLIC
            ${Library}
            Catch2::Catch2WithMain
    )

    add_custom_command(TARGET TESTS POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/static
                $<TARGET_FILE_DIR:TESTS>/static
    )

    catch_discover_tests(TESTS)
endif()
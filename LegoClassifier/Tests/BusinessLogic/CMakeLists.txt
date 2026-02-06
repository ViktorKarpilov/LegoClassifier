set(BUSINESS_LOGIC_TESTS_NAME BUSINESS_TESTS)

set(TESTS_SRCS
       dummy_test.cpp
)

add_executable(${BUSINESS_LOGIC_TESTS_NAME} ${TESTS_SRCS})

target_link_libraries(${BUSINESS_LOGIC_TESTS_NAME} PUBLIC
        Catch2::Catch2WithMain
        ${BUSINESS_LOGIC_LIB}
)

catch_discover_tests(${BUSINESS_LOGIC_TESTS_NAME})

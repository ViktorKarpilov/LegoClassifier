#include "catch2/catch_test_macros.hpp"
#include <dummy_logic.h>

TEST_CASE("FAILED DUMMY test case", "[dymmy]")
{
    REQUIRE(1 == 2);
    REQUIRE(1 == 1);
}

TEST_CASE("SUCCEED DUMMY test case", "[dymmy]")
{
    REQUIRE(1 == 1);
    REQUIRE(return_zero() == 0);
}

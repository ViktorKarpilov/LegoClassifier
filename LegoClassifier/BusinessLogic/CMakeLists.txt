
set(BUSINESS_LOGIC_SRCS
        Src/dummy_logic.cpp
)

set(BUSINESS_LOGIC_INCS
        Inc/dummy_logic.h
)

add_library(${BUSINESS_LOGIC_LIB} ${BUSINESS_LOGIC_SRCS})
target_include_directories(${BUSINESS_LOGIC_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
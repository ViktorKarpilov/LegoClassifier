set(APP_LIB APP_LIB)
set(APP_LIB ${APP_LIB} PARENT_SCOPE)

set(APP_INC
        Inc
)

set(APP_SRCS
        Src/app.cpp
)


add_library(${APP_LIB}
        ${APP_SRCS})

target_include_directories(${APP_LIB} PUBLIC
        ${APP_INC}
)

target_link_libraries(${APP_LIB}
        ${PLATFORM_LIB}
)

set(PLATFORM_LIB PLATFORM_LIB)
set(PLATFORM_LIB ${PLATFORM_LIB} PARENT_SCOPE)

set(INC_DIRECTORIES
        Inc
)

#set(DRIVERS_DIRS
#        ${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
#        ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
#        ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include
#)

# Camera OV2640
set(OV2640_HEADERS_DIRS
        Camera/OV2640
)
set(OV2640_SRCS
        Camera/OV2640/ov2640.cpp
        Camera/OV2640/ov2640_regs.cpp
)

# Board WeActStudioMiniSTM
set(WeActStudioMiniSTM_HEADERS_DIRS
        STM32/WeActStudioMiniSTM
)
set(WeActStudioMiniSTM_SRCS
        STM32/WeActStudioMiniSTM/weact_mini_stm.cpp
)

set(MCU_HEADERS_DIRS
        STM32/STM32H723VGT6
)
set(MCU_SRCS
        STM32/STM32H723VGT6/stm32.cpp
)

set(CAMERA_SRCS
        Camera/camera.cpp
)

add_library(${PLATFORM_LIB}
            ${WeActStudioMiniSTM_SRCS}
            ${OV2640_SRCS}
            ${CAMERA_SRCS}
            ${MCU_SRCS}
)

target_include_directories(${PLATFORM_LIB} PUBLIC ${INC_DIRECTORIES})

target_include_directories(${PLATFORM_LIB} PRIVATE
                           ${OV2640_HEADERS_DIRS}
                           ${WeActStudioMiniSTM_HEADERS_DIRS}
                           ${MCU_HEADERS_DIRS}
)

target_include_directories(${PLATFORM_LIB} PRIVATE )

target_link_libraries(${PLATFORM_LIB} PRIVATE stm32cubemx)

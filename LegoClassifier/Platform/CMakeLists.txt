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

# ST7735 LCD
set(ST7735_HEADERS_DIRS
        Display/ST7735
)
set(ST7735_SRCS
        Display/ST7735/logo_128_160.c
        Display/ST7735/logo_160_80.c
        Display/ST7735/st7735.c
        Display/ST7735/st7735_reg.c
)

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
set(DISPLAY_SRCS
        Display/lcd.cpp
)
set(DISPLAY_DIRS
        Display
)

add_library(${PLATFORM_LIB}
            ${WeActStudioMiniSTM_SRCS}
            ${OV2640_SRCS}
            ${CAMERA_SRCS}
            ${ST7735_SRCS}
            ${MCU_SRCS}
            ${DISPLAY_SRCS}
)

target_include_directories(${PLATFORM_LIB} PUBLIC ${INC_DIRECTORIES})

target_include_directories(${PLATFORM_LIB} PRIVATE
                           ${OV2640_HEADERS_DIRS}
                           ${WeActStudioMiniSTM_HEADERS_DIRS}
                           ${MCU_HEADERS_DIRS}
                           ${ST7735_HEADERS_DIRS}
                           ${DISPLAY_DIRS}
)

target_include_directories(${PLATFORM_LIB} PRIVATE )

target_link_libraries(${PLATFORM_LIB} PRIVATE stm32cubemx)

cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(cabel_puller C CXX ASM)

set(MAIN_NAME cabel_puller)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(cabel_puller
    main.c
)

target_link_libraries(cabel_puller
    pico_stdlib
    hardware_pwm
    hardware_clocks
)

pico_add_extra_outputs(${MAIN_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)


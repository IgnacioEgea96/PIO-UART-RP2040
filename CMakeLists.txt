cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(uart_pio C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(pio_uart)

add_executable(${PROJECT_NAME} 
    main.c
)

pico_add_extra_outputs(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    pio_uart
    hardware_uart
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

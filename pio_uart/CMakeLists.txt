add_library(pio_uart INTERFACE)

pico_generate_pio_header(pio_uart ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)
pico_generate_pio_header(pio_uart ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)

target_include_directories(pio_uart
    INTERFACE
    ./include)

target_sources(pio_uart
    INTERFACE
    uart_pio.c
)

target_link_libraries(pio_uart
    INTERFACE
    hardware_irq
    hardware_pio
)

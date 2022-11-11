#ifndef UART_RX
#define UART_RX

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include "uart_tx.pio.h"
#include "uart_rx.pio.h"

typedef void (*uart_rx_handler_t)(uint8_t data);

void uart_rx_init(PIO pio, uint sm, uint pin, uint baudrate, uint irq);
void uart_tx_init(PIO pio, uint sm, uint pin, uint baud);
void uart_rx_set_handler(uart_rx_handler_t handler);
void uart_tx_putc(PIO pio, uint sm, char c);
void uart_tx_puts(PIO pio, uint sm, const char *s);

#endif

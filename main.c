/**
 * -------------------------------------------------------------------------------
 * 
 *  A pio program that implements a simple uart protocol for the RP2040 over PIO
 *  Also configures the hardware UART0
 *  Prints recieved chars over USB
 *  TX_PIN:GPIO3, RX_PIN:GPIO4
 * -------------------------------------------------------------------------------
 */

#include "uart_pio.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define PIO_TX_PIN 2
#define PIO_RX_PIN 3
#define PIO_RX_IRQ PIO0_IRQ_0 // values for pio0: PIO0_IRQ_0, PIO0_IRQ_1. values for pio1: PIO1_IRQ_0, PIO1_IRQ_1

static int chars_rxed = 0;

// RX interrupt handler for Hardware UART
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if (uart_is_writable(UART_ID)) {
            printf("%c",ch);
        }
        chars_rxed++;
    }
}

// RX interrupt handler for PIO UART
static void rx_handler(uint8_t data)
{
    printf("%c",data);
}

void init_hardware_uart(){
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);
    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);
    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // Turn ON FIFO's
    uart_set_fifo_enabled(UART_ID, true);
    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
    sleep_ms(1000);
}

int main()
{
    stdio_init_all();
    sleep_ms(2000);
    init_hardware_uart();
    printf("Starting example PIO UART\n");
    sleep_ms(2000);
    printf("TX GPIO: %d, RX GPIO: %d\n\n", PIO_TX_PIN, PIO_RX_PIN);
    sleep_ms(2000);

    //Definitions for PIO State Machines
	PIO pio = pio0;
    uint sm_rx = pio_claim_unused_sm(pio, true);
    uint sm_tx = pio_claim_unused_sm(pio, true);

    //PIO State machines initialization
    uart_tx_init(pio, sm_tx, PIO_TX_PIN, BAUD_RATE);
    uart_rx_init(pio, sm_rx, PIO_RX_PIN, BAUD_RATE, PIO_RX_IRQ);

    //Set RX handler function
    uart_rx_set_handler(rx_handler);

    while (true)
    {
        uart_tx_puts(pio, sm_tx, "Hi from PIO UART\n");
        sleep_ms(1000);
        uart_tx_puts(pio, sm_tx, "Bye from PIO UART\n");
        sleep_ms(1000);
    }
}

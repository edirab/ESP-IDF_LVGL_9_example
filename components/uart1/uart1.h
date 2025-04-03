#ifndef UART1_APP_IDF_H_
#define UART1_APP_IDF_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <string.h>

/*
    Pins for UART1 must be assigned.
    Lets connect them to 
        D19 -> TXD
        D21 -> RXD
*/

#define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
#define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (CONFIG_EXAMPLE_UART_PORT_NUM)
#define ECHO_UART_BAUD_RATE     (CONFIG_EXAMPLE_UART_BAUD_RATE)
#define ECHO_TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)

#define UART1_BUF_SIZE (1024)

// Configure a temporary buffer for the incoming data
extern uint8_t *uart1_data;

void setup_uart1();

void uart_echo_task();

// sends "get_wifi_credentials"
void uart_send_provision_wifi_command();

#endif // UART1_APP_IDF_H_

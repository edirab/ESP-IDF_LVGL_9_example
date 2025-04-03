#include "uart1.h"

uint8_t *uart1_data;

void setup_uart1() 
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, UART1_BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Configure a temporary buffer for the incoming data
    uart1_data = (uint8_t *) malloc(UART1_BUF_SIZE);
}

void uart_echo_task()
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        // Read data from the UART
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, uart1_data, (UART1_BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        // ESP_ERROR_CHECK(len);

        if (len > 0) {
            // Write data back to the UART
            uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) uart1_data, len);
            uart1_data[len] = '\0';
            ESP_LOGI(__FUNCTION__, "Recv str: %s", (char *) uart1_data);
        }
    }

}

void uart_send_provision_wifi_command() {
    strcpy((const char*) uart1_data, "get_wifi_credentials");
    int len = strlen((const char*) uart1_data);
    uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) uart1_data, len);
    ESP_LOGI(__FUNCTION__, "Sent %s", (const char* ) uart1_data);
}

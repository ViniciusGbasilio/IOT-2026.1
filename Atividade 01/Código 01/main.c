#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 7

void app_main() {

    // Configuração dos pinos como saída
    gpio_reset_pin(LED1);
    gpio_reset_pin(LED2);
    gpio_reset_pin(LED3);
    gpio_reset_pin(LED4);

    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED3, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED4, GPIO_MODE_OUTPUT);

    while (1) {

        // Contador binário de 0 a 15
        for(int contador = 0; contador < 16; contador++) {

            gpio_set_level(LED1, contador % 2);
            gpio_set_level(LED2, (contador / 2) % 2);
            gpio_set_level(LED3, (contador / 4) % 2);
            gpio_set_level(LED4, (contador / 8) % 2);

            vTaskDelay(500 / portTICK_PERIOD_MS);
        }

        
        gpio_set_level(LED1, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED1, 0);
        gpio_set_level(LED2, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED2, 0);
        gpio_set_level(LED3, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED3, 0);
        gpio_set_level(LED4, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED4, 0);

        gpio_set_level(LED4, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED4, 0);
        gpio_set_level(LED3, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED3, 0);
        gpio_set_level(LED2, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED2, 0);
        gpio_set_level(LED1, 1); vTaskDelay(400 / portTICK_PERIOD_MS); gpio_set_level(LED1, 0);
    }
}
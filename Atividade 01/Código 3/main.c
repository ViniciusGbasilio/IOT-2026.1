#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/uart.h"

#define LED2 4
#define LED1 1
#define BOTAO1 18
#define BOTAO2 19
#define BUZZER 5

#define UART_PORT UART_NUM_0
#define BUF_SIZE 1024

void app_main()
{
    gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);

    gpio_set_direction(BOTAO1, GPIO_MODE_INPUT);
    gpio_set_direction(BOTAO2, GPIO_MODE_INPUT);

    gpio_set_pull_mode(BOTAO1, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(BOTAO2, GPIO_PULLDOWN_ONLY);

    // PWM para buzzer
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 2000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num = BUZZER,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0
    };
    ledc_channel_config(&channel);

    // Configuração UART
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_PORT, &uart_config);
    uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);

    int estado_led1 = 0;
    int estado_led2 = 0;
    int contador = 0;
    int botaoB_habilitado = 1;

    uint8_t data[1];

    while(1)
    {
        // leitura UART
        int len = uart_read_bytes(UART_PORT, data, 1, pdMS_TO_TICKS(10));

        if(len > 0)
        {
            if(data[0] == 'a')
            {
                botaoB_habilitado = 0;
                printf("Botao B desativado\n");
            }
            else if(data[0] == 'b')
            {
                botaoB_habilitado = 1;
                printf("Botao B ativado\n");
            }
        }

        // BOTAO 1 -> alterna LED1
        if(gpio_get_level(BOTAO1) == 1)
        {
            estado_led1 = !estado_led1;
            gpio_set_level(LED1, estado_led1);
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        // BOTAO 2 -> buzzer 1.5s (se habilitado)
        if(gpio_get_level(BOTAO2) == 1 && botaoB_habilitado)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 128);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

            vTaskDelay(pdMS_TO_TICKS(1500));

            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

            vTaskDelay(pdMS_TO_TICKS(300));
        }

        // LED2 piscando a cada 2s
        contador += 20;

        if(contador >= 2000)
        {
            estado_led2 = !estado_led2;
            gpio_set_level(LED2, estado_led2);
            contador = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
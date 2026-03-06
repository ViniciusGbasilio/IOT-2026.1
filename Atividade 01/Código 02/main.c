#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 7
#define BUZZER 8

#define MODE LEDC_LOW_SPEED_MODE
#define LED_TIMER LEDC_TIMER_0
#define BUZZER_TIMER LEDC_TIMER_1

void app_main(void)
{

    // timer dos leds
    ledc_timer_config_t led_timer = {
        .speed_mode = MODE,
        .timer_num = LED_TIMER,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&led_timer);

    // timer do buzzer
    ledc_timer_config_t buzzer_timer = {
        .speed_mode = MODE,
        .timer_num = BUZZER_TIMER,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 500,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&buzzer_timer);


    // canais dos leds
    ledc_channel_config_t leds[4] = {

        {.channel = LEDC_CHANNEL_0, .gpio_num = LED1, .speed_mode = MODE, .timer_sel = LED_TIMER, .duty = 0},
        {.channel = LEDC_CHANNEL_1, .gpio_num = LED2, .speed_mode = MODE, .timer_sel = LED_TIMER, .duty = 0},
        {.channel = LEDC_CHANNEL_2, .gpio_num = LED3, .speed_mode = MODE, .timer_sel = LED_TIMER, .duty = 0},
        {.channel = LEDC_CHANNEL_3, .gpio_num = LED4, .speed_mode = MODE, .timer_sel = LED_TIMER, .duty = 0}

    };

    for(int i=0;i<4;i++){
        ledc_channel_config(&leds[i]);
    }


    // canal buzzer
    ledc_channel_config_t buzzer = {
        .channel = LEDC_CHANNEL_4,
        .gpio_num = BUZZER,
        .speed_mode = MODE,
        .timer_sel = BUZZER_TIMER,
        .duty = 512
    };

    ledc_channel_config(&buzzer);


    int duty = 0;
    int dir = 1;

    int freq = 500;
    int freq_dir = 1;


    while(1)
    {

        // ---------- FADING LEDS ----------

        duty += dir * 5;

        if(duty >= 1023){
            duty = 1023;
            dir = -1;
        }

        if(duty <= 0){
            duty = 0;
            dir = 1;
        }

        for(int i=0;i<4;i++){
            ledc_set_duty(MODE, leds[i].channel, duty);
            ledc_update_duty(MODE, leds[i].channel);
        }


        // ---------- BUZZER ----------

        freq += freq_dir * 20;

        if(freq >= 2000){
            freq = 2000;
            freq_dir = -1;
        }

        if(freq <= 500){
            freq = 500;
            freq_dir = 1;
        }

        ledc_set_freq(MODE, BUZZER_TIMER, freq);


        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
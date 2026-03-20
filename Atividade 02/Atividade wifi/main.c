#include <stdio.h>
#include <string.h>
#include <time.h>

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_sntp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// -------- CONFIGURAÇÕES --------
#define WIFI_SSID      "Wokwi-GUEST"
#define WIFI_PASS      ""
#define MAX_RETRIES    10
#define SYNC_DELAY_MS  2000
#define PRINT_DELAY_MS 5000

static const char *TAG = "SNTP_APP";

// -------- WIFI --------

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
int32_t event_id,void *event_data)
{
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } 
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            ESP_LOGW(TAG, "Wi-Fi desconectado, reconectando...");
            esp_wifi_connect();
        }
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Wi-Fi conectado com sucesso!");
    }
}

static void wifi_init(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        &instance_any_id);

    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &wifi_event_handler,
                                        NULL,
                                        &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        }
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

// -------- SNTP --------

static void sntp_initialize(void)
{
    ESP_LOGI(TAG, "Inicializando SNTP...");

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "a.ntp.br");
    esp_sntp_setservername(1, "b.ntp.br");
    esp_sntp_setservername(2, "c.ntp.br");

    esp_sntp_init();
}

static void obtain_time(void)
{
    sntp_initialize();

    time_t now = 0;
    struct tm timeinfo = {0};

    for (int i = 0; i < MAX_RETRIES; i++) {

        time(&now);
        localtime_r(&now, &timeinfo);

        if (timeinfo.tm_year >= (2020 - 1900)) {
            ESP_LOGI(TAG, "Horário sincronizado!");
            return;
        }

        ESP_LOGI(TAG, "Aguardando sincronização... (%d/%d)", i + 1, MAX_RETRIES);
        vTaskDelay(pdMS_TO_TICKS(SYNC_DELAY_MS));
    }

    ESP_LOGW(TAG, "Falha ao sincronizar horário.");
}

// -------- TEMPO --------

static void print_time(void)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    printf("Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n",
           timeinfo.tm_mday,
           timeinfo.tm_mon + 1,
           timeinfo.tm_year + 1900,
           timeinfo.tm_hour,
           timeinfo.tm_min,
           timeinfo.tm_sec);
}

// -------- MAIN --------

void app_main(void)
{
    // Inicializa NVS
    nvs_flash_init();

    // Inicia Wi-Fi
    wifi_init();

    // Aguarda estabilizar conexão
    vTaskDelay(pdMS_TO_TICKS(5000));

    // Sincroniza horário
    obtain_time();

    // Configura timezone Brasil
    setenv("TZ", "BRT3", 1);
    tzset();

    // Loop principal
    while (1) {
        print_time();
        vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY_MS));
    }
}
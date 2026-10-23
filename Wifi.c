#include "Wifi.h"
#include <string.h>
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

void Khoi_tao_wifi(void)
{
    esp_err_t ket_qua = nvs_flash_init();
    if(ket_qua == ESP_ERR_NVS_NO_FREE_PAGES || ket_qua == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cau_hinh_goc = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cau_hinh_goc);
    wifi_config_t cau_hinh_wifi = {
        .ap = {
            .ssid = "Xe_can_bang",
            .ssid_len = strlen("Xe_can_bang"),
            .password = "12345678",
            .max_connection = 2,
            .authmode = WIFI_AUTH_WPA2_PSK
        },
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &cau_hinh_wifi);
    esp_wifi_start();
    printf("Da phat wifi\n");
    printf("Ten: %s \nPass: %s\n", "Xe_can_bang", "12345678");
}
#include "TCP_server.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"

static void tcp_server_task(void *pvParameters)
{
    char bo_dem_nhan[128];
    struct sockaddr_in dia_chi_server;
    dia_chi_server.sin_addr.s_addr = htonl(INADDR_ANY);
    dia_chi_server.sin_family = AF_INET;
    dia_chi_server.sin_port = htons(4444);
    int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    bind(listen_socket, (struct sockaddr*)&dia_chi_server, sizeof(dia_chi_server));
    listen(listen_socket, 1);
    printf("TCP server dang mo o cong: %d.\n", 4444);
    while(1)
    {
        struct sockaddr_in dia_chi_laptop;
        socklen_t do_dai_dia_chi = sizeof(dia_chi_laptop);
        int socket_ket_noi = accept(listen_socket, (struct sockaddr *)&dia_chi_laptop, &do_dai_dia_chi);
        printf("Ket noi thanh cong.\n");
        while(1)
        {
            int do_dai_du_lieu = recv(socket_ket_noi, bo_dem_nhan, sizeof(bo_dem_nhan)-1, 0);
            if(do_dai_du_lieu<=0)
            {
                printf("Laptop da ngat ket noi. Cho ket noi lai...\n");
                break;
            }
            bo_dem_nhan[do_dai_du_lieu]=0;
            printf("Laptop gui lenh: %s\n", bo_dem_nhan);
            char bo_dem_gui[64];
            sprintf(bo_dem_gui, "ESP32 da nhan lenh.\n");
            send(socket_ket_noi, bo_dem_gui, strlen(bo_dem_gui), 0);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        close(socket_ket_noi);
    }
}

void khoi_tao_tcp_server(void)
{
    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}
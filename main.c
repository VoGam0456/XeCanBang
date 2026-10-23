#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "MPU9250.h"
#include "L298N.h"
#include "Wifi.h"
#include "TCP_server.h"

typedef struct {
    float kp;
    float ki;
    float kd;
    
    float error;
    float last_error;
    float integral;
} PID_Controller;

PID_Controller motor_pid = {
    .kp = 15.5f,  
    .ki = 0.0f,   
    .kd = 1.2f,   
    .error = 0,
    .last_error = 0,
    .integral = 0
};

float tinh_toan_pid(float setpoint, float goc_thuc_te, float dt) {
    motor_pid.error = setpoint - goc_thuc_te;
    float p_out = motor_pid.kp * motor_pid.error;
    motor_pid.integral += motor_pid.error * dt;
    if (motor_pid.integral > 100.0f) motor_pid.integral = 100.0f;
    if (motor_pid.integral < -100.0f) motor_pid.integral = -100.0f;
    float i_out = motor_pid.ki * motor_pid.integral;
    float derivative = (motor_pid.error - motor_pid.last_error) / dt;
    float d_out = motor_pid.kd * derivative;
    motor_pid.last_error = motor_pid.error;
    return p_out + i_out + d_out;
}
void vTask_DieuKhienXe(void *pvParameters) {
    i2c_master_dev_handle_t the_mpu = *(i2c_master_dev_handle_t *)pvParameters;
    mpu_du_lieu_tho tho;
    mpu_da_xu_li da_xu_li;
    float goc_xe = 0.0f;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10);
    while (1) {
        if (lay_du_lieu_mpu(the_mpu, &tho, &da_xu_li) == ESP_OK) {
            goc_xe = tinh_goc(da_xu_li, goc_xe, 0.01f); 
            float toc_do_dong_co = tinh_toan_pid(0.0f, goc_xe, 0.01f);
            int pwm_output = (int)toc_do_dong_co;
            dieu_khien_2_banh(pwm_output, pwm_output);
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
void app_main(void) {
    Khoi_tao_wifi();
    khoi_tao_tcp_server();
    khoi_tao_dong_co();
    i2c_master_dev_handle_t the_mpu; 
    if (thiet_lap_i2c_va_mpu(&the_mpu) == ESP_OK) {
        printf("Ket noi MPU9250 thanh cong.\n");
    } else {
        printf("Khong tim thay MPU9250.\n");
        return; 
    }
    xTaskCreate(vTask_DieuKhienXe, "Task_DieuKhienXe", 4096, &the_mpu, 5, NULL);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
#pragma once
#include <stdint.h>
#include "driver/i2c_master.h"
#include <math.h>

typedef struct {
    int16_t giatoc_x;
    int16_t giatoc_y;
    int16_t giatoc_z;
    int16_t vantoc_x;
    int16_t vantoc_y;
    int16_t vantoc_z;
} mpu_du_lieu_tho;

typedef struct {
    float giatoc_x; 
    float giatoc_y;
    float giatoc_z;
    float vantoc_x;
    float vantoc_y;
    float vantoc_z;
} mpu_da_xu_li;

esp_err_t khoi_tao_mpu(i2c_master_dev_handle_t thiet_bi_mpu);
esp_err_t lay_du_lieu_mpu(i2c_master_dev_handle_t thiet_bi_mpu, mpu_du_lieu_tho* tho, mpu_da_xu_li* da_xu_li);
esp_err_t thiet_lap_i2c_va_mpu(i2c_master_dev_handle_t* the_mpu);
float tinh_goc(mpu_da_xu_li da_xu_li, float goc_hien_tại, float thoi_gian);
#include "MPU9250.h"

esp_err_t khoi_tao_mpu(i2c_master_dev_handle_t thiet_bi_mpu)
{
    uint8_t khoi_tao[2] = {0x6B, 0x00};
    return i2c_master_transmit(thiet_bi_mpu, khoi_tao, 2, -1);
}

esp_err_t lay_du_lieu_mpu(i2c_master_dev_handle_t thiet_bi_mpu, mpu_du_lieu_tho* tho, mpu_da_xu_li* da_xu_li)
{
    uint8_t thanh_ghi_bat_dau = 0x3B;
    uint8_t du_lieu_mang[14];
    esp_err_t ket_qua = i2c_master_transmit_receive(
        thiet_bi_mpu,
        &thanh_ghi_bat_dau, 1,
        du_lieu_mang, 14,
        -1
    );
    if(ket_qua == ESP_OK){
        tho->giatoc_x = (int16_t)((du_lieu_mang[0] << 8) | du_lieu_mang[1]);
        tho->giatoc_y = (int16_t)((du_lieu_mang[2] << 8) | du_lieu_mang[3]);
        tho->giatoc_z = (int16_t)((du_lieu_mang[4] << 8) | du_lieu_mang[5]);
        tho->vantoc_x = (int16_t)((du_lieu_mang[8] << 8) | du_lieu_mang[9]);
        tho->vantoc_y = (int16_t)((du_lieu_mang[10] << 8) | du_lieu_mang[11]);
        tho->vantoc_z = (int16_t)((du_lieu_mang[12] << 8) | du_lieu_mang[13]);
        da_xu_li->giatoc_x = tho->giatoc_x / 16384.0;
        da_xu_li->giatoc_y = tho->giatoc_y / 16384.0;
        da_xu_li->giatoc_z = tho->giatoc_z / 16384.0;
        da_xu_li->vantoc_x = tho->vantoc_x / 131.0;
        da_xu_li->vantoc_y = tho->vantoc_y / 131.0;
        da_xu_li->vantoc_z = tho->vantoc_z / 131.0;
    }
    return ket_qua;
}

esp_err_t thiet_lap_i2c_va_mpu(i2c_master_dev_handle_t* the_mpu) {
    i2c_master_bus_config_t cau_hinh_bus = {};
    cau_hinh_bus.i2c_port = I2C_NUM_0;
    cau_hinh_bus.sda_io_num = (gpio_num_t)22;
    cau_hinh_bus.scl_io_num = (gpio_num_t)21;
    cau_hinh_bus.clk_source = I2C_CLK_SRC_DEFAULT;
    cau_hinh_bus.glitch_ignore_cnt = 7;
    cau_hinh_bus.flags.enable_internal_pullup = true;
    i2c_master_bus_handle_t duong_truyen;
    esp_err_t loi = i2c_new_master_bus(&cau_hinh_bus, &duong_truyen);
    if (loi != ESP_OK) return loi;
    i2c_device_config_t cau_hinh_mpu = {};
    cau_hinh_mpu.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    cau_hinh_mpu.device_address = 0x68;
    cau_hinh_mpu.scl_speed_hz = 400000;
    loi = i2c_master_bus_add_device(duong_truyen, &cau_hinh_mpu, the_mpu);
    if (loi != ESP_OK) return loi;
    return khoi_tao_mpu(*the_mpu); 
}

float tinh_goc(mpu_da_xu_li da_xu_li, float goc_hien_tai, float thoi_gian)
{
    float goc_gia_toc = atan2(da_xu_li.giatoc_y, da_xu_li.giatoc_z)*(180/M_PI);
    float goc_van_toc = goc_hien_tai + ((da_xu_li.vantoc_x+1.6)*thoi_gian);
    float goc_nghieng = 0.96*goc_van_toc + 0.04*goc_gia_toc;
    return goc_nghieng;
}
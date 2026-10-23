#include "L298N.h"

void khoi_tao_dong_co() {
    gpio_set_direction(16, GPIO_MODE_OUTPUT);
    gpio_set_direction(17, GPIO_MODE_OUTPUT);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);
    gpio_set_direction(23, GPIO_MODE_OUTPUT);
    ledc_timer_config_t timer_cfg = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .timer_num       = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz         = 1000,
        .clk_cfg         = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_cfg);
    ledc_channel_config_t ch_trai = {
        .gpio_num   = 18, 
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&ch_trai);
    ledc_channel_config_t ch_phai = {
        .gpio_num   = 5,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_1,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&ch_phai);
}

int gioi_han_toc_do(int toc_do){
    if (toc_do > 255) return 255;
    if (toc_do < -255) return -255;
    return toc_do;
}

void chay_banh_trai(int toc_do){
    toc_do = gioi_han_toc_do(toc_do); 

    if (toc_do >= 0) {
        gpio_set_level(16, 1); 
        gpio_set_level(17, 0); 
    } else {         
        gpio_set_level(16, 0); 
        gpio_set_level(17, 1); 
    }
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, abs(toc_do));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void chay_banh_phai(int toc_do) {
    toc_do = gioi_han_toc_do(toc_do);

    if (toc_do >= 0) { 
        gpio_set_level(19, 1); 
        gpio_set_level(23, 0); 
    } else {          
        gpio_set_level(19, 0); 
        gpio_set_level(23, 1); 
    }
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, abs(toc_do));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}
void dieu_khien_2_banh(int toc_do_trai, int toc_do_phai) {
    chay_banh_trai(toc_do_trai);
    chay_banh_phai(toc_do_phai);
}

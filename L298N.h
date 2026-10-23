#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include <stdlib.h> 

void khoi_tao_dong_co();
int gioi_han_toc_do(int);
void chay_banh_trai(int);
void chay_banh_phai(int);
void dieu_khien_2_banh(int, int);

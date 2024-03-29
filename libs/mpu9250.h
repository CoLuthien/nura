#pragma once
#include "sensor.h"
#include "i2c.h"
#include <stdbool.h>

#define MPU9250_ADDR 0x68

typedef struct mpu9250
{/*singleton object!*/
    sensor_t super;
    float accel_res, gyro_res;

}mpu9250_t;


mpu9250_t* init_mpu9250(i2c_dev_t* i2c, int sample_rate, uint8_t accel_scale, uint16_t gyro_scale);



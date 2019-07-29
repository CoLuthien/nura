#include "mpu9250.h"
#include "mpu9250_reg.h"
#include "debug.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Accelerometer and xoscope self test; check calibration wrt factory settings
bool self_test(mpu9250_t* self)
{
    i2c_dev_t* i2c = self->super.comm;
    uint8_t FS = 0;

    float a_sample[3] = {0.0}, g_sample[3] = {0.0}, a_st[3] = {0.0}, g_st[3] = {0.0}, a_str[3] = {0.0}, g_str[3] = {0.0};
    float ast_val[3], gst_val[3], a_res[3] , g_res[3];
    ASSERT(i2c->set_addr(i2c, self->super.device_addr) > -1);
    uint8_t ast_data[3], gst_data[3];
    
    uint8_t sample_rate, config, gyro_config, accel_conf1, accel_conf2;

    sample_rate = i2c->read_byte_reg(i2c, SMPLRT_DIV);
    config = i2c->read_byte_reg(i2c, CONFIG);
    gyro_config = i2c->read_byte_reg(i2c, GYRO_CONFIG);
    accel_conf1 = i2c->read_byte_reg(i2c, ACCEL_CONFIG_1);
    accel_conf2 = i2c->read_byte_reg(i2c, ACCEL_CONFIG_2);

    //setting a low pass filter
    i2c->write_bit_reg(i2c, CONFIG, 2, 3, 2, true);//trouble point
    i2c->write_bit_reg(i2c, ACCEL_CONFIG_2, 2, 3, 2, false);

    //set fchoice bit
    i2c->write_bit_reg(i2c, GYRO_CONFIG, 1, 1, 0b00, true);
    i2c->write_bit_reg(i2c, ACCEL_CONFIG_2, 3, 1, 0b0, true);


    i2c->write_bit_reg(i2c, GYRO_CONFIG, 4, 2, 0b00, true);//set scale to 250dps
    i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 4, 2, 0b00, true);//set scale to 2g

    for(int i = 0; i < 200; i++)
    {
        a_sample[0] += (float)((int16_t)(i2c->read_byte_reg(i2c, ACCEL_XOUT_H) << 8 |i2c->read_byte_reg(i2c, ACCEL_XOUT_L)));
        a_sample[1] += (float)((int16_t)(i2c->read_byte_reg(i2c, ACCEL_YOUT_H) << 8 |i2c->read_byte_reg(i2c, ACCEL_YOUT_L)));
        a_sample[2] += (float)((int16_t)(i2c->read_byte_reg(i2c, ACCEL_ZOUT_H) << 8 |i2c->read_byte_reg(i2c, ACCEL_ZOUT_L)));

        g_sample[0] += (float)((int16_t)(i2c->read_byte_reg(i2c, GYRO_XOUT_H) << 8 |i2c->read_byte_reg(i2c, GYRO_XOUT_L)));
        g_sample[1] += (float)((int16_t)(i2c->read_byte_reg(i2c, GYRO_YOUT_H) << 8 |i2c->read_byte_reg(i2c, GYRO_YOUT_L)));
        g_sample[2] += (float)((int16_t)(i2c->read_byte_reg(i2c, GYRO_ZOUT_H) << 8 |i2c->read_byte_reg(i2c, GYRO_ZOUT_L)));
    }
    
    for(int i = 0; i < 3; i++)
    {
        a_sample[i] /= 200.0;
        g_sample[i] /= 200.0;
    }

    
    i2c->write_bit_reg(i2c, GYRO_CONFIG, 7, 3, 0b111, true);
    i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 7, 3, 0b111, true);


    usleep(40*1000);

    for(int i = 0; i < 200; i++)
    {
        a_st[0] += (float)((int16_t)(i2c->read_byte_reg(i2c, ACCEL_XOUT_H) << 8 |i2c->read_byte_reg(i2c, ACCEL_XOUT_L)));
        a_st[1] += (float)((int16_t)(i2c->read_byte_reg(i2c, ACCEL_YOUT_H) << 8 |i2c->read_byte_reg(i2c, ACCEL_YOUT_L)));
        a_st[2] += (float)((int16_t)(i2c->read_byte_reg(i2c, ACCEL_ZOUT_H) << 8 |i2c->read_byte_reg(i2c, ACCEL_ZOUT_L)));

        g_st[0] += (float)((int16_t)(i2c->read_byte_reg(i2c, GYRO_XOUT_H) << 8 |i2c->read_byte_reg(i2c, GYRO_XOUT_L)));
        g_st[1] += (float)((int16_t)(i2c->read_byte_reg(i2c, GYRO_YOUT_H) << 8 |i2c->read_byte_reg(i2c, GYRO_YOUT_L)));
        g_st[2] += (float)((int16_t)(i2c->read_byte_reg(i2c, GYRO_ZOUT_H) << 8 |i2c->read_byte_reg(i2c, GYRO_ZOUT_L)));
    }

     for(int i = 0; i < 3; i++)
    {
        a_st[i] /= 200.0;
        g_st[i] /= 200.0;
    }

    for(int i = 0; i < 3; i++)
    {
        a_str[i] = a_st[i] - a_sample[i];
        g_str[i] = g_st[i] - g_sample[i];
    }

    i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 7, 3, 0b000, true);
    i2c->write_bit_reg(i2c, GYRO_CONFIG, 7, 3, 0b000, true);

    usleep(40 * 1000);

    ast_data[0] = i2c->read_byte_reg(i2c, SELF_TEST_X_ACCEL);
    ast_data[1] = i2c->read_byte_reg(i2c, SELF_TEST_Y_ACCEL);
    ast_data[2] = i2c->read_byte_reg(i2c, SELF_TEST_Z_ACCEL);

    gst_data[0] = i2c->read_byte_reg(i2c, SELF_TEST_X_GYRO);
    gst_data[1] = i2c->read_byte_reg(i2c, SELF_TEST_Y_GYRO);
    gst_data[2] = i2c->read_byte_reg(i2c, SELF_TEST_Z_GYRO);

    for(int i = 0; i < 3; i++)
    {
        ast_val[i] = (float)(2620.0/ (1 <<FS)) * powf(1.01, (float)ast_data[i] - 1.0);
        gst_val[i] = (float)(2620.0/(1 << FS)) * powf(1.01, (float)gst_data[i] - 1.0);
    }

    for(int i = 0; i < 3; i++)
    {
        a_res[i] = a_str[i] / ast_val[i];
        g_res[i] = g_str[i] / gst_val[i];
    }
    for (int i = 0; i < 3; i++)
    {
        if(g_res[i] < 0.5)
            return false;
        printf("%d th  percentage is : a: %f g: %f \n", i, a_res[i], g_res[i]);
    }
    //restore original setting
    i2c->write_byte_reg(i2c, SMPLRT_DIV, sample_rate);
    i2c->write_byte_reg(i2c, CONFIG, config);
    i2c->write_byte_reg(i2c, GYRO_CONFIG, gyro_config);
    i2c->write_byte_reg(i2c, ACCEL_CONFIG_1, accel_conf1);
    i2c->write_byte_reg(i2c, ACCEL_CONFIG_2, accel_conf2);
    return true;
}



static void _init_mpu9250(mpu9250_t* self)
{
    comm_device_t* com = self->super.comm;
    uint8_t packet = 0x00;
    float test_result[6] = {0, 0, 0, 0, 0, 0};
    ASSERT(com != NULL);
    ASSERT(com->type == I2C);

    i2c_dev_t* i2c = (i2c_dev_t*)com;

    if (i2c->set_addr(i2c, self->super.device_addr) != 0)
    {
        printf("failed to set address to %p", self->super.device_addr);
        return;
    }
    if(self->super.rate != 0)
    {
        printf("sorry rate is not implemented. set to 50hz\n");
        self->super.rate = 50;
    }
    

    i2c->write_bit_reg(i2c, PWR_MGMT_1, 6, 1, 0b0, true);// turn off sleep mode 
    i2c->write_bit_reg(i2c, PWR_MGMT_1, 2, 3, 0b001, true);// select automatic clock source
    i2c->write_bit_reg(i2c, PWR_MGMT_2, 5, 6, 0b000000, true);// turn on gyro and accel


    i2c->write_byte_reg(i2c, SMPLRT_DIV, 0x00); // data output rate / 1 + samplerate 

    i2c->write_bit_reg(i2c, GYRO_CONFIG, 7, 3, 0b000, true);// disable self test mode

    i2c->write_bit_reg(i2c, GYRO_CONFIG, 1, 2, 0b00, true);// select fchoice_b, in this case we will use lpf
    i2c->write_bit_reg(i2c, CONFIG, 2, 3, 0b101, true); //select gyro low pass filter range 20ms term

    i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 7, 3, 0b000, true);//disable  self test mode
   
    i2c->write_bit_reg(i2c, ACCEL_CONFIG_2, 3, 1, 0b0, true);
    i2c->write_bit_reg(i2c, ACCEL_CONFIG_2, 2, 3, 0b100, true);// select fchoice_b and accel lpf range

    i2c->write_bit_reg(i2c, USER_CTRL, 6, 1, 0, true);//disable fifo
    i2c->write_bit_reg(i2c, USER_CTRL, 5, 1, 0, true);//disable i2c_mst function 
    i2c->write_byte_reg(i2c, FIFO_EN, 0x00);//disable fifo for all 


    usleep(40 * 1000); 
    //check we are really commnuicate with mpu9250
    if (i2c->read_byte_reg(i2c, WHO_AM_I) != 0x71)
    {
        printf("device not respond!!\n");
        return;
    }

    if(!self_test(self))
    {
        printf("self test failed!\n");
    }
    return;
}

mpu9250_t* init_mpu9250(i2c_dev_t* i2c, int sample_rate, uint8_t accel_scale, uint16_t gyro_scale)
{
    mpu9250_t* self = malloc(sizeof(mpu9250_t));
    sensor_t* super = &self->super;
    super->comm = i2c;
    super->rate = sample_rate;//Hz 
    super->device_addr = MPU9250_ADDR;

    i2c->write_bit_reg(i2c, PWR_MGMT_1, 7, 1, 1, false); //reset device
    usleep(100*1000);

    switch (accel_scale)
    {
        case 2:
            self->accel_res = (float)accel_scale / 32768.0f;
            i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 4, 2, 0b00, true);// select accel scale, +-4g
            break;    
        case 4:
            self->accel_res = (float)accel_scale / 32768.0f;
            i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 4, 2, 0b01, true);// select accel scale, +-4g
            break;    
        case 8:
            self->accel_res = (float)accel_scale / 32768.0f;
            i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 4, 2, 0b10, true);// select accel scale, +-4g
            break;    
        case 16:
            self->accel_res = (float)accel_scale / 32768.0f;
            i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 4, 2, 0b11, true);// select accel scale, +-4g
            break;    
        default:
            self->accel_res = 8.0f / 32768.0f;
            i2c->write_bit_reg(i2c, ACCEL_CONFIG_1, 4, 2, 0b10, true);// select accel scale, +-4g
            printf("accel scale you want does not exist!! setup to 8g defaults\n");// todo: change log system
            break;
    }

    switch (gyro_scale)
    {
        case 250:
            self->gyro_res = (float)gyro_scale / 32768.0f;
            i2c->write_bit_reg(i2c, GYRO_CONFIG, 4, 2, 0b00, true);// select gyro scale 250dps(degree per second)
            break;
        case 500:
            self->gyro_res = (float)gyro_scale / 32768.0f;
            i2c->write_bit_reg(i2c, GYRO_CONFIG, 4, 2, 0b01, true);// select gyro scale 250dps(degree per second)
            break;
        case 1000:
            self->gyro_res = (float)gyro_scale / 32768.0f;
            i2c->write_bit_reg(i2c, GYRO_CONFIG, 4, 2, 0b10, true);// select gyro scale 250dps(degree per second)
            break;   
        case 2000:
            self->gyro_res = (float)gyro_scale / 32768.0f;
            i2c->write_bit_reg(i2c, GYRO_CONFIG, 4, 2, 0b11, true);// select gyro scale 250dps(degree per second)
            break;
        default:
            self->gyro_res = (float)500.0 / 32768.0f;
            printf("gyro scale you want does not exist!! setup to default value 500deg/s\n");
            break;
    }
    
    _init_mpu9250(self);

    return self;
}

static inline void check_conn(mpu9250_t* self)
{
    i2c_dev_t* i2c = self->super.comm;
    ASSERT(i2c->set_addr(i2c, self->super.device_addr) > -1);
    ASSERT(i2c->read_byte_reg(i2c, WHO_AM_I) == MPU9250_ID);
}

void update_data(mpu9250_t* self)
{
    i2c_dev_t* i2c = self->super.comm;
    ASSERT(i2c->super.type == I2C);
    check_conn(self);

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    i2c->read_nbyte_reg(i2c, ACCEL_YOUT_H, 2, &ay);
    i2c->read_nbyte_reg(i2c, ACCEL_XOUT_H, 2, &ax);
    i2c->read_nbyte_reg(i2c, ACCEL_ZOUT_H, 2, &az);

    i2c->read_nbyte_reg(i2c, GYRO_XOUT_H, 2, &gx);
    i2c->read_nbyte_reg(i2c, GYRO_YOUT_H, 2, &gy);
    i2c->read_nbyte_reg(i2c, GYRO_ZOUT_H, 2, &gz);

    self->accel[0] = (float)ax * self->accel_res;
    self->accel[1] = (float)ay * self->accel_res;
    self->accel[2] = (float)az * self->accel_res;

    self->gyro[0] = (float)gx * self->gyro_res;
    self->gyro[1] = (float)gy * self->gyro_res;
    self->gyro[2] = (float)gz * self->gyro_res;
}

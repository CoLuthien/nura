#pragma once
#include "comm_device.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <stdint.h>

typedef struct _i2c_device_t
{
    comm_device_t super;
    uint8_t dev_addr;

    int (*set_addr)(struct _i2c_device_t* self, uint8_t addr);
    uint8_t (*get_addr)(struct _i2c_device_t* self);
    
    int (*read_byte_reg)(struct _i2c_device_t* self, uint8_t reg);
    int (*read_nbyte_reg)(struct _i2c_device_t* self, uint8_t reg, size_t len, uint8_t* buffer);
 // true : maintain value, false: erase value
    int (*write_bit_reg)(struct _i2c_device_t*, uint8_t reg, uint8_t position, uint8_t len, uint8_t data, bool mode); 
   
    int (*write_byte_reg)(struct _i2c_device_t* self, uint8_t reg, uint8_t data);
    int (*write_nbyte_reg)(struct _i2c_device_t* self, uint8_t reg, size_t len, uint8_t* buffer);
}i2c_dev_t;

i2c_dev_t* init_i2c(const char* device_name);

int open_device(const char* device_name);
int close_device(i2c_dev_t* self);
static int check_functions(int fd, unsigned int func_code);
int set_address(i2c_dev_t* self, int addr);
uint8_t get_address(i2c_dev_t* self);
static inline int i2c_access(int fd, char rw, uint8_t command, int size, union i2c_smbus_data* data);

int i2c_write_bit_reg(i2c_dev_t* self, uint8_t reg, uint8_t pos, uint8_t len, uint8_t data, bool mode);

int i2c_read_byte(i2c_dev_t* self);
int i2c_write_byte(i2c_dev_t* self, uint8_t data);

int i2c_read_byte_reg(i2c_dev_t* self, uint8_t reg);
int i2c_read_nbyte_reg(i2c_dev_t* self, uint8_t reg, size_t len, uint8_t* buffer);

int i2c_write_byte_reg(i2c_dev_t* self, uint8_t reg, uint8_t data);
int i2c_write_nbyte_reg(i2c_dev_t* self, uint8_t reg, size_t len, uint8_t* buffer);


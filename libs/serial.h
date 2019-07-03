#pragma once
#include "comm_device.h"
#include "attr.h"
#include <termios.h>


typedef struct serial_dev_t
{
    comm_device_t super;
    speed_t baud;

    void (*data_flush)(struct serial_dev_t* self);
}serial_dev_t;

serial_dev_t* init_serial(const char* serial_name, int baudrate);
int serial_open(const char*, int);
void close_serial(serial_dev_t* self);
void data_flush(serial_dev_t* self);

int serial_read(serial_dev_t* self);
int serial_nread(serial_dev_t* self, size_t len, uint8_t* buffer);

int serial_write(serial_dev_t* self, uint8_t data);
int serial_nwrite(serial_dev_t* self, size_t len, uint8_t* buffer);















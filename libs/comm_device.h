#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

enum comm_type 
{
	I2C, 
	UART,
	SERIAL,
	NON
};
typedef struct comm_device // interface of communication devices 
{
	int fd;
	enum comm_type type;
	int (*close_device)(struct comm_device* self);

	int (*read_byte)(struct comm_device* self);
	int (*read_nbyte)(struct comm_device* self, size_t len, uint8_t* buffer);

	int (*write_byte)(struct comm_device* self, uint8_t data);
	int (*write_nbyte)(struct comm_device* self, size_t len, uint8_t* buffer);
}comm_device_t;
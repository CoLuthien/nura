#pragma once
#include <pthread.h>
#include "list.h"
#include "comm_device.h"
#include <sys/types.h>


typedef struct _sensor
{
	comm_device_t* comm;
	int device_addr;
	unsigned int rate;
}sensor_t; 
	
	

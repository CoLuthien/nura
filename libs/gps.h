#pragma once
#include "serial.h"
#include "sensor.h"
#include <stdint.h>
#include "nmea/include/nmea.h"
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>



typedef struct _gps_t{
    sensor_t super;
    nmeaINFO cur_info;
    nmeaPARSER parser;

}gps_t;

gps_t* init_gps(serial_dev_t* uart);

void gps_store(gps_t* self);





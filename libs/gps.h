#include "serial.h"
#include "./nmea/include/nmea.h"
#include <stdbool.h>



typedef struct _gps_t
{
    serial_dev_t* port;
    char* buffer;
    unsigned int position;
    nmeaINFO cur_info;
    nmeaPARSER parser;
}gps_t;

gps_t* init_gps(serial_dev_t* port, unsigned int len);
void destroy_gps(gps_t* gps);

bool gps_try_receive(gps_t* gps);





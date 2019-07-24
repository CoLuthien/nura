#include "gps.h"
#include "serial.h"

serial_dev_t* port = NULL;
gps_t* gps = NULL;
int main()
{
    port = init_serial("/dev/...", 115200);
    gps = init_gps(port, 512);

    while(1)
    {
        if(gps_try_receive(gps))
        {
            nmeaINFO* info = &gps->cur_info;
            printf("%f %f %f", info->lat, info->lon, info->elv);
        }

    }
}
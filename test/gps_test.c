#include "gps.h"
#include "serial.h"
#include <stdio.h>
#include <unistd.h>
serial_dev_t* port = NULL;
gps_t* gps = NULL;
int main()
{
    port = init_serial("/dev/ttyAMA0", 9600);
    gps = init_gps(port);

    while(1)
    {
	    gps_store(gps);
    	usleep(1000*1000);
    }
}

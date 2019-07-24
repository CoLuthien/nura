#include "gps.h"
#include <string.h>


gps_t* init_gps(serial_dev_t* uart)
{
    gps_t* self = malloc(sizeof(gps_t));
    sensor_t* super = &self->super;

    super->rate = 10;
    super->comm = uart;
    self->raw_gps_data = fopen("raw_gpsdata", "a+");

    return self;
}

void gps_store(gps_t* self)
{
    serial_dev_t* uart = (serial_dev_t*)self->super.comm;
    uint8_t idx = 0;;
    uint8_t stat;
    uint8_t ch;

    uint8_t arr[256];

	memset(arr, 0x00, 256);
    //TO DO: this method can discard the packet need another method
    while((ch = uart->super.read_byte(uart)) != 0x0a && idx < 256)
    {
	if(ch != -1)
	{
        	arr[idx] = ch;
        	idx += 1;
	}
	if(ch == 255)
	{	
		break;
 	}

    }
	int len = nmea_parse(&self->parser, arr, idx, &self->cur_info);	
	if(idx != 0)
	{
		printf("%f %f\n", self->cur_info.lat, self->cur_info.lon);
		printf("%s\n", arr);
	}

}

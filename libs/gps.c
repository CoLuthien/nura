#include "gps.h"
#include <string.h>


gps_t* init_gps(serial_dev_t* port)
{

    //gps is cold start!
    gps_t* self = malloc(sizeof(gps_t));
    sensor_t* super = &self->super;

    super->rate = 10;
    super->comm = port;

    const char* update_rate = "$PMTK220,100*2F\r\n";
    const char* baud_rate = "$PMTK251,115200*18\r\n";

    if(port->baud != 9600)
    {
        printf("gps baud rate does not match :%d\n", port->baud);
        port->update_baud(port, 9600);
    }
    port->super.write_nbyte(port, strlen(baud_rate), baud_rate);
    port->update_baud(port, 115200);

    port->super.write_nbyte(port, strlen(update_rate), update_rate);

    nmea_parser_init(&self->parser);
    nmea_zero_INFO(&self->cur_info);

    printf("gps port baudrate: %d\n", port->baud);

    return self;
}

void gps_receive(gps_t* self)
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
		if(ch != 0)
		{
				arr[idx] = ch;
				idx += 1;
		}
        if(idx > 255 || ch == 0)
        {
            break;
        }
        
    }
	int len = nmea_parse(&self->parser, (const char*)arr, strlen(arr), &self->cur_info);	
}

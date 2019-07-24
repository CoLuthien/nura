#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gps.h"


gps_t* init_gps(serial_dev_t* port, unsigned int len)
{
    gps_t* self = malloc(sizeof(gps_t));
    sensor_t* super = &self->super;
	super->comm = port
    self->position = 0;
    nmea_parser_init(&self->parser);
    nmea_zero_INFO(&self->cur_info);

    return self;
}

void destroy_gps(gps_t* gps)
{
    free(gps->buffer);
    gps->buffer = NULL;
    nmea_parser_destroy(&gps->parser);
    free(gps);
    gps = NULL;
}

bool gps_try_receive(gps_t* self)
{
    serial_dev_t* s = &self->super.comm

    uint8_t ch;
    unsigned int* pos = &self->position;
	while((ch = s->super.read_byte(s)) != 0x0a && *pos < 256)
	{
		if(ch != -1)
			putchar(ch);
		*pos+=1;
	}
	*pos =0;
return true;

}

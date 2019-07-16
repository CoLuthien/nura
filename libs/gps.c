#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gps.h"


gps_t* init_gps(serial_dev_t* port, unsigned int len)
{
    gps_t* self = malloc(sizeof(gps_t));
    self->port = port;
    self->position = 0;
    self->buffer = malloc(sizeof(char)* len);
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
    serial_dev_t* s = self->port;
    int8_t ch;
    unsigned int* pos = &self->position;
    do 
    {
        ch = s->super.read_byte(&s->super);
        if (ch == -1)
        {
            return false;
        }
        self->buffer[*pos + 1] = (char)ch;
        *pos += 1;
    }while(ch != '\n');

    int len = nmea_parse(&self->parser, self->buffer, self->position, &self->cur_info);
    memset(self->buffer, 0x00, *pos);
    *pos = 0;
    if(len == *pos)
    {
        return true;
    }
    return false;
}

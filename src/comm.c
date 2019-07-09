#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "comm.h"
#define END 0b10101010
com_t* init_com(serial_dev_t* serial)
{
    com_t* self = malloc(sizeof(com_t));
    self->used = 0;
    self->port = serial;
    memset(self->outbound, NULL, 64);

    return self;
}
void destroy_com(com_t* com)
{
    send_to_gcs(com);// flush remaining logs

    com->port->super.close_device(com->port);

    free(com);
    com = NULL;    
}

void sendto(com_t* self, char* buf)
{
    size_t len = strlen(buf);
    char* heap = malloc(len);
    memcpy(heap, buf, len);

    self->outbound[self->used] = heap;
    self->used += 1;
}

static void recv_frm_gcs(com_t* self)
{
    serial_dev_t* port = self->port;

    uint8_t ch;
    uint8_t cnt = 0;
    uint8_t buf[64] = {0,};
    do
    {
        ch = port->super.read_byte(port);
        buf[cnt] = ch;
        cnt += 1;
    } while (ch != END && cnt <= 63);
    // now we have to parse the received packets
    /*
        TODO!!
     */
}

static void send_to_gcs(com_t* self)
{
    serial_dev_t* port = self->port;
    for(unsigned int i = 0; i < self->used; i++)
    {
        port->super.write_nbyte
                (port, strlen(self->outbound[i]), self->outbound[i]);
    }
}
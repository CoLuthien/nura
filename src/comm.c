#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "comm.h"

/*
    todo:
     implement checksum. 
     add function pointer at com_t struct. 
     input length check
 */
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

static int make_pkt(char* pkt, char* payload)
{
    

}






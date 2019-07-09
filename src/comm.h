#include <fcntl.h>
#include <pthread.h>
#include "../libs/serial.h"

/*
    our protocol
    -------------...
    |id|command|data|checksum|end point
     1, 1, 1~60, 1, 1 byte
    -------
 */

typedef struct _com_t
{
    serial_dev_t* port;
    char* outbound[64];
    unsigned int used;// length is static
}com_t;

bool is_full(com_t* self);

com_t* init_com(serial_dev_t* serial);
void destroy_com(com_t* com);
void sendto(com_t* self, char* buf);

static void recv_frm_gcs(com_t* self);
static void send_to_gcs(com_t* self);




#include <fcntl.h>
#include <pthread.h>
#include "../libs/serial.h"

/*
    our protocol
    -------------...
    |start,id, command, len, data, checksum|
    1, 1, 1, 1, len, 1 byte
    assume len <= 255
    -------
 */

typedef struct _com_t
{
    serial_dev_t* port;
    char* outbound[64];
    unsigned int used;// length is static
}com_t;

typedef struct packet
{
    uint8_t start;
    uint8_t id;
    uint8_t command;
    uint8_t len;
    char* payload;
    uint8_t checksum;
}packet_t;

bool is_full(com_t* self);

com_t* init_com(serial_dev_t* serial);
void destroy_com(com_t* com);
void sendto(com_t* self, char* buf);

static void recv_frm_gcs(com_t* self);
static void send_to_gcs(com_t* self);




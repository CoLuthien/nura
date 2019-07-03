#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _logger_t
{
    FILE* fp;
    char** buffer;
    unsigned int used;
    unsigned int length;
    unsigned int interval;
}logger_t;

logger_t* init_logger(const char* file_name, unsigned int len,
    unsigned int interval);
void destroy_logger(logger_t* self);

void push_log(logger_t* self, char* log);
void flush_log(logger_t* self);

#include "log.h"

logger_t* init_logger(const char* file_name, unsigned int len)
{
    logger_t* log = (logger_t*)malloc(sizeof(logger_t));
    log->fp = fopen(file_name, "a");
    log->buffer = malloc(sizeof(char*) * len);
    log->length = len;
    log->used = 0;

    return log;
}

void destroy_logger(logger_t* self)
{
   
    if(self->used != 0)
    {
        flush_log(self);
    }
    ASSERT(0 == self->used);
    free(self->buffer);

    fclose(self->fp);
    free(self);
}

void push_log(logger_t* self, char* log)
{
    if((self->used + 1) >= self->length)
    {
        flush_log(self);
    }
    *(self->buffer + self->used) = log;
    self->used += 1;
}

void flush_log(logger_t* self)
{
    if(0 == self->used)
    {
        return;
    }
    for(unsigned int i = 0; i < self->used; i++)
    {
        fprintf(self->fp, "%s\n", *(self->buffer + i));
    }
    self->used = 0;
}
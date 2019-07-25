#include "sensor.h"
#include "i2c.h"

typedef struct _lps25_t
{
    sensor_t super;
    i2c_dev_t* i2c;
    float pressure, temp;
}lps25_t;

lps25_t* init_baro(i2c_dev_t* i2c, int rate);
void destroy_baro(lps25_t* self);
void update_baro(lps25_t* self);


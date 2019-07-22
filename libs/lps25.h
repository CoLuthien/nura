#include "sensor.h"
#include "i2c.h"

typedef struct _lps25_t
{
    sensor_t super;
    i2c_dev_t* i2c;
    
    

}lps25_t;
#include <bcm2835.h>


typedef struct _pwm_t
{
    


}pwm_t;

void set_clock(uint32_t divisor);
void set_data(uint8_t chan, uint32_t data);
void set_mode()
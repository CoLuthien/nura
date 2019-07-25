#include "lps25.h"
#include "lps25_reg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool check_conn(lps25_t* self, uint8_t addr)
{
    i2c_dev_t* i2c = self->super.comm;

    i2c->set_addr(i2c, addr);
    uint8_t val = i2c->read_byte_reg(i2c, BARO_WHO_AM_I);
    
    return val == BARO_WAI_VAL;
}


lps25_t* init_baro(i2c_dev_t* i2c)
{
    lps25_t* self = malloc(sizeof(lps25_t));
    self->super.comm = i2c;


    if(!check_conn(self, LPS25_ADDR1) && !check_conn(self, LPS25_ADDR2))
    {
        printf("barometer does not response!!\n");
        return NULL;
    }

    i2c->write_bit_reg(i2c,CTRL_REG1, 7, 1, 1, true);// set power on
    i2c->write_bit_reg(i2c, CTRL_REG2, 7, 1, 0, true); // set boot time reset

    usleep(40* 1000);// wait 40ms vim

    i2c->write_bit_reg(i2c, CTRL_REG2, 3, 1, 0, true); // enable i2c
    i2c->write_bit_reg(i2c, CTRL_REG2, 1, 1, 1, true);// set auto zero value
    return self;
}

void destroy_baro(lps25_t* self)
{
    free(self);
}

void update_baro(lps25_t* self)
{
    i2c_dev_t* i2c = self->super.comm;
    uint8_t prs[3];

    i2c->read_nbyte_reg(i2c, PRESS_OUT_XL, 2, prs);

    unsigned int p = prs[2] << 16 | prs[1] << 8 | prs[0];
    self->pressure = (float)(p / 4096);

    i2c->read_nbyte_reg(i2c, TEMP_OUT_L, 2, prs);
    unsigned int t = prs[1] << 8 | prs[0];    
    self->temp = (float)(t / 480) + 42.5f;
}


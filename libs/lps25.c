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
    printf("initializing barometer!!\n");

    if(!check_conn(self, LPS25_ADDR1) && !check_conn(self, LPS25_ADDR2))
    {
        printf("barometer does not response!!\n");
        return NULL;
    }

    i2c->write_bit_reg(i2c, CTRL_REG2, 7, 1, 1, true); // set boot time reset
    i2c->write_bit_reg(i2c, CTRL_REG2, 2, 1, 1, true);
    usleep(400*1000);

    i2c->write_bit_reg(i2c,CTRL_REG1, 7, 1, 1, true);// set power on
    i2c->write_bit_reg(i2c, CTRL_REG1, 1, 1, 0, true);// noreset autozero

    i2c->write_bit_reg(i2c, CTRL_REG1, 6, 3, 0b001, true); // set update_rate
    i2c->write_bit_reg(i2c, CTRL_REG1, 2, 1, 1, true);//set block data update

    i2c->write_bit_reg(i2c, RES_CONF, 1, 2, 0b10, true);

    usleep(200* 1000);// wait 200ms 

    i2c->write_bit_reg(i2c, CTRL_REG2, 3, 1, 0, true); // enable i2c
    i2c->write_bit_reg(i2c, CTRL_REG2, 1, 1, 1, true);// set auto zero value
    
    i2c->write_bit_reg(i2c, CTRL_REG2, 6, 3, 0, true);
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

    prs[0] = i2c->read_byte_reg(i2c, PRESS_OUT_XL);
    prs[1] = i2c->read_byte_reg(i2c, PRESS_OUT_L);
    prs[2] = i2c->read_byte_reg(i2c, PRESS_OUT_H);

    uint32_t ref_p = (uint32_t)(
		    	i2c->read_byte_reg(i2c, REF_P_H)<< 16 |
			i2c->read_byte_reg(i2c, REF_P_L)<< 8 |
			i2c->read_byte_reg(i2c, REF_P_XL));

    int32_t p = (int32_t)(
		    i2c->read_byte_reg(i2c, PRESS_OUT_H) << 24  |
	       	    i2c->read_byte_reg(i2c, PRESS_OUT_L) << 16 |
		    i2c->read_byte_reg(i2c, PRESS_OUT_XL) << 8
		    );
    p = p >> 8;
	printf("%x\n", p);


    self->pressure = (float)((float)p / 4096.0f);

    uint16_t t = (uint16_t)(
		    i2c->read_byte_reg(i2c,TEMP_OUT_H) << 8|
		    i2c->read_byte_reg(i2c, TEMP_OUT_L) );

    self->temp = (float)(t / 480) + 42.5f;
}


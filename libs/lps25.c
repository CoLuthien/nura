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


lps25_t* init_baro(i2c_dev_t* i2c, int update_rate)
{
    lps25_t* self = malloc(sizeof(lps25_t));
    self->super.comm = i2c;
    printf("initializing barometer!!\n");
    self->super.rate = update_rate;
    self->super.device_addr = LPS25_ADDR1; // WE ONLY USE 1 BAROMETER

    if(!check_conn(self, LPS25_ADDR1) && !check_conn(self, LPS25_ADDR2))
    {
        printf("barometer does not response!!\n");
        return NULL;
    }

    i2c->write_bit_reg(i2c, CTRL_REG2, 7, 1, 1, true); // set boot time reset
    i2c->write_bit_reg(i2c, CTRL_REG2, 2, 1, 0, true); // no diff, actual value;
    usleep(400*1000);

    i2c->write_bit_reg(i2c,CTRL_REG1, 7, 1, 1, true);// set power on
    i2c->write_bit_reg(i2c, CTRL_REG1, 1, 1, 0, true);// noreset autozero
    

    switch (update_rate)
    {
        case 1:
            i2c->write_bit_reg(i2c, CTRL_REG1, 6, 3, 0b001, true); // set update_rate
            break;
        case 7:
            i2c->write_bit_reg(i2c, CTRL_REG1, 6, 3, 0b010, true); // set update_rate
            break;
        case 13:
            i2c->write_bit_reg(i2c, CTRL_REG1, 6, 3, 0b011, true); // set update_rate
            break;
        case 25:
            i2c->write_bit_reg(i2c, CTRL_REG1, 6, 3, 0b100, true); // set update_rate
            break;
        default:
            i2c->write_bit_reg(i2c, CTRL_REG1, 6, 3, 0b001, true);
            break;
    }

   
    i2c->write_bit_reg(i2c, CTRL_REG1, 2, 1, 1, true);//set block data update

    //set average resolution
    i2c->write_bit_reg(i2c, RES_CONF, 1, 2, 0b11, true);
    i2c->write_bit_reg(i2c, RES_CONF, 3, 2, 0b11, true);

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
    i2c->set_addr(i2c, self->super.device_addr);

    uint32_t ref_p = (uint32_t)(
		    	i2c->read_byte_reg(i2c, REF_P_H)<< 16 |
			i2c->read_byte_reg(i2c, REF_P_L)<< 8 |
			i2c->read_byte_reg(i2c, REF_P_XL));

    int32_t p = (int32_t)(
		    i2c->read_byte_reg(i2c, PRESS_OUT_H) << 24  |
	       	    i2c->read_byte_reg(i2c, PRESS_OUT_L) << 16 |
		    i2c->read_byte_reg(i2c, PRESS_OUT_XL) << 8
		    );
            /*
            for RPi 3B+
            int x = 1;   //000000000......01
            x = x << 31; //100000000......00
            x = x >> 31; //111111111......11 (Everything is filled with 1s !!!!!) 
            so we will use this property
             */
    p = p >> 8;


    self->pressure = (float)((float)p / 4096.0f);

    int16_t t = (int16_t)(
		    i2c->read_byte_reg(i2c,TEMP_OUT_H) << 8|
		    i2c->read_byte_reg(i2c, TEMP_OUT_L) );

    self->temp = (float)(t / 480) + 42.5f;
}


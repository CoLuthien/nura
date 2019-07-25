#include "lps25.h"
#include "lps25_reg.h"
#include <stdio.h>
#include <stdlib.h>

 int i2c_read_bit_reg(i2c_dev_t* self, uint8_t reg, uint8_t pos, uint8_t len)
{
    uint8_t packet = self->read_byte_reg(self, reg);
     uint8_t mask = ((1 << len) -1) << (pos - len + 1);
     /* 2**n - 1
18      =
			   219      ....0 1    1   1   1 ...
			   220          n n-1  n-2 n-3   ...
			   221 
			   222     ex 
			   223         1111 1011 = original data 
			   224         0000 0111 = mask
			   225         3 = position 
			   226         3 = len 
			   227         we need to move 3rd bit just 1 time to left         
			   228     */

     packet &= (mask);
     return packet >> pos;
}



int main()
{
	i2c_dev_t* i2c = init_i2c("/dev/i2c-1");
	printf("%p\n", *i2c->write_bit_reg);

	lps25_t* baro = init_baro(i2c);
	printf("%p\n", baro);
	int i = 0; 		
	while(i < 20)
	{
		i++;
	printf("\n\n");
		printf("before:%d %d\n",i2c_read_bit_reg(i2c, STAT_REG, 5,1), i2c_read_bit_reg(i2c, STAT_REG, 1, 1));

		update_baro(baro);
		printf("pt :%f %f\n", baro->pressure, baro->temp);
	
		printf("after:%d %d\n",i2c_read_bit_reg(i2c, STAT_REG, 5,1), i2c_read_bit_reg(i2c, STAT_REG, 1, 1));

		usleep(1000*1000);
	}
	return 0;
}

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#define IN 0
#define OUT 1

#define LOW 0
#define HIGH 1

int export_gpio(int pin);
int unexport_gpio(int pin);
int set_gpio_direction(int pin, int dir);
int read_gpio(int pin);
int write_gpio(int pin, int value);
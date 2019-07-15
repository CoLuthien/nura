CC = gcc
CFLAGS = -Wall -c 
DEPEND_FILE = depend_file.mk

OBJS = gpio.o i2c.o log.o mpu9250.o task.o serial.o list.o 
SRCS = $(OBJS:%.o=%.c)


LIBS= -lm


all: depend target

target: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o  $(OBJS)


.PHONY: clean

clean: 
	rm $(OBJS) $(DEPEND_FILE)
depend: 
	$(CC) -MM $(SRCS) > $(DEPEND_FILE)

-include $(DEPEND_FILE)


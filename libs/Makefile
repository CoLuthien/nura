CC = gcc
CFLAGS = -Wall -c 
DEPEND_FILE = depend_file.mk

OBJS = gpio.o i2c.o log.o mpu9250.o task.o serial.o list.o lps25.o gps.o
SRCS = $(OBJS:%.o=%.c)
OBJDIR = build/


LIBS= -lm -llibnmea


all: depend $(OBJDIR) move nmealib

$(OBJDIR): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o  $(OBJS)

move: 
	mv *.o ../build/

nmealib: 
	cd nmea && $(MAKE)


.PHONY: clean

clean: 
	rm $(OBJS) $(DEPEND_FILE)
	
depend: 
	$(CC) -MM $(SRCS) > $(DEPEND_FILE)

-include $(DEPEND_FILE)


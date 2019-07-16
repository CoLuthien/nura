

CC = gcc 
CFLAGS= -Wall -c
SRCS = main.c
OBJS = $(SRCS:%.c = %.o)
INCS = -I ../libs -I ../libs/nmea

all: depend
	$(CC) $(CFLAGS) $(INCS) $(OBJS)

depend: 
	$(CC) -MM $(INCS) $(SRCS) > depend_file.mk

-include depend_file.mk
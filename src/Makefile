

CC = gcc 
CFLAGS= -Wall -c
SRCS = main.c
OBJS = $(SRCS:%.c = %.o)
INCS = -I ../libs -I ../libs/nmea

all: depend bin move

bin:
	$(CC) $(CFLAGS) $(INCS) $(OBJS)

depend: 
	$(CC) -MM $(INCS) $(SRCS) > depend_file.mk

move:
	mv *.o ../build/

-include depend_file.mk
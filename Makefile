CC = gcc
CFLAGS = -Wall
#$* <- 확장자가 없는 현재의 목표 파일(Target)

#$@ <- 현재의 목표 파일(Target)

#$< <- 현재의 목표 파일(Target)보다 더 최근에 갱신된 파일 이름

#$? <- 현재의 목표 파일(Target)보다 더 최근에 갱신된 파일이름

OBJS = main.o gpio.o gps.o i2c.o list.o log.o lps25.o mpu9250.o serial.o task.o
BUILD_DIR = build

TARGET_OBJS = $(OBJS:%.o=$(BUILD_DIR)/%.o)

INCS = -I ../libs -I ../libs/nmea
LIBS = -lm  

all: libs logic
	$(CC) $(CFLAGS) -o main.out $(INCS) $(TARGET_OBJS) $(BUILD_DIR)/libnmea.a $(LIBS)


libs: 
	cd libs && $(MAKE) 

logic:
	cd src && $(MAKE)

depend: 
	$(CC) -MM $(INCS) $(SRCS) > depend_file.mk

-include depend_file.mk
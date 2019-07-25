#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "debug.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>



serial_dev_t* init_serial(const char* serial_name, int baudrate) 
{
	comm_device_t* super = malloc(sizeof(serial_dev_t));
	serial_dev_t* self = (serial_dev_t*)super;

	super->fd = serial_open(serial_name, baudrate);
	if(super->fd == -1)
	{
		printf("fd acquire failed\n");
		return NULL;
	}
	super->type = UART;

	super->read_byte = serial_read;
	super->read_nbyte = serial_nread;

	super->write_byte = serial_write;
	super->write_nbyte = serial_nwrite;
	super->close_device = close_serial;

	self->data_flush = data_flush;
	self->update_baud = update_baud;
	self->baud = baudrate;

	return self;
}
void close_serial(serial_dev_t* self)
{
	ASSERT(self->super.type == UART);

	close(self->super.fd);
	free(self);
	self = NULL;
}

void update_baud(serial_dev_t* self, int baud)
{
	struct termios options;
	tcgetattr(self->super.fd, &options);

	cfsetispeed(&options, baud);
	cfsetospeed(&options, baud);

	tcsetattr(self->super.fd, TCSANOW, &options);
	self->baud = baud;

	return;
}

int serial_open(const char* name, int baudrate)
{
	struct termios options;
	speed_t baud;
	int stat, fd;
	
	switch(baudrate)
	{
		case 9600: baud = B9600; break;
		case 19200: baud = B19200; break;
		case 38400: baud = B38400; break;
		case 57600: baud = B57600; break;
		case 115200: baud = B115200; break;
		case 230400: baud = B230400; break;
		
		default:
 			return -2;
	}
	fd = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		return -1;

	tcgetattr(fd, &options);
	cfmakeraw(&options);
	cfsetispeed(&options, baud);
	cfsetospeed(&options, baud);

	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;

	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	options.c_lflag &= ~(ICANON | ECHO | ECHOE| ISIG);
	options.c_oflag &= ~OPOST;
	
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	tcsetattr(fd, TCSANOW, &options);

	ioctl (fd, TIOCMGET, &stat);

	stat |= TIOCM_DTR;
	stat |= TIOCM_RTS;
	ioctl(fd, TIOCMSET, &stat);
	return fd;
}


int serial_read(serial_dev_t* self)
{
	int received;
	int code = read(self->super.fd, &received, 1);
	
	return (code > 0) ? (uint8_t)received: -1;// consider the byte is 
}

int serial_nread(serial_dev_t* self, size_t len, uint8_t* buffer)
{
	int received = read(self->super.fd, (void*)buffer, len);
	return received;
}

int serial_write(serial_dev_t* self, uint8_t data)
{
	return write(self->super.fd, &data, 1);
}

int serial_nwrite(serial_dev_t* self, size_t len, uint8_t* buffer)
{
	return write(self->super.fd, buffer, len);
}

void data_flush(serial_dev_t* self)
{
	tcflush(self->super.fd, TCIFLUSH);
}

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>                             
#include "Uart.h"

int uart_open(char *path, int oflags)
{
	int fd;
	struct termios options;

	// Open device
	if((fd = open(path, oflags)) == -1)
	{
		return -1;
	}

	// Intialization uart port
	
	fcntl(fd,F_SETFL,0);
	//fcntl(fd, F_SETFL, FNDELAY); //ne pas bloquer sur le read
	tcgetattr(fd,&options);
	usleep(10000);
	cfsetospeed(&options,B9600);
	cfsetispeed(&options,B9600);
	options.c_cflag &= ~PARENB; /* Parite   : none */
	options.c_cflag &= ~CSTOPB; /* Stop bit : 1    */
	options.c_cflag &= ~CSIZE;  /* Bits     : 8    */
	options.c_cflag |= CS8;
	options.c_cflag &= ~CRTSCTS;
	// options.c_iflag &= ~(IXON);
	// tcsetattr(fd,TCSANOW,&options);
	options.c_lflag &= ~OPOST;
	options.c_lflag &= ~(ICANON | ECHO | ECHONL|IEXTEN | ISIG);
	// c_cc
	options.c_cc[VMIN]= 1;
	options.c_cc[VTIME]= 4;
	//flush!
	//tcflush(fd,TCIOFLUSH);
	tcsetattr(fd, TCSANOW,&options);
	tcflush(fd,TCIOFLUSH);
	usleep(10000);

	return fd;
}

void uart_close(int fd)
{
	// Close device
	close(fd);
}

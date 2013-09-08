#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "Uart.h"
#include "GPSDevice.h"

static int gps_dev_fd = -1;

int gps_dev_open(char *devName)
{
	gps_dev_fd = uart_open(devName, GPS_DEV_O_FLAGS);

	if(gps_dev_fd == -1)
	{
		// Fail to open device
		return -1;
	}

	return 0;
}

void gps_dev_close()
{
	uart_close(gps_dev_fd);
	gps_dev_fd = -1;
}

int gps_dev_read(char *msg, int size)
{
	char	ch;
	int	cnt = 0;	

	if(size < GPS_DEV_MSG_SIZE)
	{
		// The size is not enough
		return -1;
	}

	// Ignore the non-complete message
	do {
		if(read(gps_dev_fd, &ch, 1) < 1)
			return -1; // read error
		
	} while(ch != '$');

	// Read a complete message
	do {
		if(read(gps_dev_fd, &ch, 1) < 1)
			return -1; // read error

		if(ch != '\005')
			msg[cnt++] = ch;

	} while(ch != '\n'); // The end of a complete message	 

	msg[cnt - 1] = '\0';

	return cnt;
}

int gps_dev_read_rmc(char *msg, int size)
{
	int cnt;
	char msgType[6] = "";

	while(strcmp(msgType, "GPRMC") != 0)
	{
		if((cnt = gps_dev_read(msg, size)) == -1)
			return -1; // read error
		
		strncpy(msgType, msg, 5);	
		msgType[5] = '\0';
	}

	return cnt;
}

int gps_dev_read_gga(char *msg, int size)
{
	int cnt;
	char msgType[6] = "";

	while(strcmp(msgType, "GPGGA") != 0)
	{
		if((cnt = gps_dev_read(msg, size)) == -1)
			return -1; // read error
		
		strncpy(msgType, msg, 5);	
		msgType[5] = '\0';
	}

	return cnt;
}

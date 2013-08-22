#include <stdio.h>
#include "GPSDevice.h"

#define GPS_MSG_SIZE 80
int main()
{
	char msg[GPS_MSG_SIZE];
	int i;

	if(gps_dev_open() == -1)
	{
		printf("Error: fail to open GPS device");
		return -1;
	}

	for(i = 0; i < 10; i++)
	{
		gps_dev_read(msg, GPS_MSG_SIZE);
		printf("%s\n", msg);
	}

	for(i = 0; i < 10; i++)
	{
		gps_dev_read_rmc(msg, GPS_MSG_SIZE);
		printf("%s\n", msg);
	}

	gps_dev_close();

	return 0;
}

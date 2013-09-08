#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "GyroDev.h"

#define INTERVAL	10000	// microsecond

int main()
{
	char *filename = "/dev/i2c-1";
	double zAngRate;
	double yawAngle = 0;
	struct timespec tp1;
	struct timespec tp2;
	double elapsedTime;	// unit seconds
	int cnt = 0;
	double totalBias = 0;

	if(gyro_dev_open(filename) == -1)
	{
		printf("Open gyroscope error");
		return -1;
	}

	clock_gettime(CLOCK_MONOTONIC, &tp1);

	while(cnt < 10000)
	{

		usleep(INTERVAL);	

		// Calculate time interval
		clock_gettime(CLOCK_MONOTONIC, &tp2);
		elapsedTime = tp2.tv_sec - tp1.tv_sec +
			 (tp2.tv_nsec - tp1.tv_nsec) / 1000000000.0;

		gyro_dev_get_rate(&zAngRate);

		if(zAngRate > 0.5 || zAngRate < -0.5)
		{
			yawAngle += zAngRate * elapsedTime;
			printf("rate: %f, yaw angle: %f\n", zAngRate, yawAngle);
		}

		tp1.tv_sec = tp2.tv_sec;
		tp1.tv_nsec = tp2.tv_nsec;

		totalBias += zAngRate;	
		cnt += 1;
	}

	printf("total bias: %f, avg bias: %f\n", totalBias, totalBias / (double)cnt);
	gyro_dev_close();

	return 0;
}

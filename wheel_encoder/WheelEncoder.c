#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "WheelEncoder.h"

#define SMPL_INTERVAL	1000000	// Sampling interval, unit: microsecond 

int wh_fd;	// Descriptor of encoder

int main()
{
	long preEnCnt;
	long curEnCnt;
	double dis; // displacement, unit millimeter(mm)
	double vel; // velosity, unit millimeter per second
	double totalDis = 0; // Total distance
	double dif;

	struct timespec tp1;
	struct timespec tp2;
	double elapsedTime;	// second
	
	if(wh_open() != 0)
	{
		perror("failed to open /dev/encoder");
		return -1;
	}

	clock_gettime(CLOCK_MONOTONIC, &tp1);
	wh_get_count(&preEnCnt);

	while(1)
	{
		usleep(SMPL_INTERVAL);

		clock_gettime(CLOCK_MONOTONIC, &tp2);
		wh_get_count(&curEnCnt);

		elapsedTime = tp2.tv_sec - tp1.tv_sec + (tp2.tv_nsec - tp1.tv_nsec) / 1000000000.0;

		printf("preCnt: %d, curCnt: %d, dif: %d\n", preEnCnt, curEnCnt, curEnCnt - preEnCnt);
		
		dif = (curEnCnt - preEnCnt);
		if(dif < 0)
		{
			dif = dif / 2;	

		}
		// Calculate displacement and velosity
		dis = WH_CIRCUM * (dif / WH_CPR);
		vel = dis / elapsedTime;
		totalDis += dis;
	
		printf("s: %f mm, v: %f mm/s, total: %f m\n", dis, vel, totalDis / 1000);

		preEnCnt = curEnCnt;
		tp1.tv_sec = tp2.tv_sec;
		tp1.tv_nsec = tp2.tv_nsec;
	}

	wh_close();		
	
	return 0;
}

int wh_open()
{
	if ((wh_fd = open(WH_DEV_NAME, O_RDONLY)) < 0)
		return -1;

	return 0;	
}

void wh_close()
{
	close(wh_fd);
}

int wh_get_count(long *pCnt)
{
	char buf[WH_BUF_SIZE];
	int bytes;

	if ((bytes = read(wh_fd, buf, WH_BUF_SIZE)) <= 0)
		return -1; // read error

	buf[bytes] = '\0';
	*pCnt = atol(buf); // Covert string to long integer
	
	return 0;	
}

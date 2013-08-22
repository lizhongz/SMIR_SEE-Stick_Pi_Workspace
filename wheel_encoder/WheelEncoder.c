#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "WheelEncoder.h"

#define SMPL_INTERVAL	1000000	// Sampling interval, unit: microsecond 

int wh_fd;	// Descriptor of encoder

int main()
{
	long preEnCnt;
	long curEnCnt;
	double dis; // displacement, unit millimeter(mm)
	double vel; // velosity, unit millimeter per second
	
	if(wh_open() != 0)
	{
		perror("failed to open /dev/encoder");
		return -1;
	}

	wh_get_count(&preEnCnt);

	while(1)
	{
		usleep(SMPL_INTERVAL);

		wh_get_count(&curEnCnt);
		//printf("preCnt: %d, curCnt: %d \n", preEnCnt, curEnCnt);
		
		// Calculate displacement and velosity
		dis = WH_CIRCUM * ((curEnCnt - preEnCnt) / WH_CPR);
		vel = dis / SMPL_INTERVAL * 1000000;
	
		preEnCnt = curEnCnt;

		printf("s: %f mm, v: %f mm/s\n", dis, vel);
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

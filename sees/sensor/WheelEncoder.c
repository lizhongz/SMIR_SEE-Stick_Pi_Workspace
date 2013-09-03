#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "WheelEncoder.h"

static int wh_fd = -1;	// Descriptor of encoder

int wh_open(char *devName)
{
	if ((wh_fd = open(devName, O_RDONLY)) < 0)
		return -1;

	return 0;	
}

void wh_close()
{
	close(wh_fd);
	wh_fd = -1;
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

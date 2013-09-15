/* Code From  http://www.tldp.org/HOWTO/Serial-Programming-HOWTO/    
   By gary@frerking.org */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ModemDev.h"

#define MODEM_DEV_NAME "/dev/ttyUSB0"

int mdm_dev_fd = -1;
struct termios newtio;
struct termios oldtio;

int main()
{
	double dists[MDM_DEV_USONIC_NUM];

	if(modem_dev_open(MODEM_DEV_NAME) == -1)
	{
		return -1;
	}

	int i;
	for(i = 0; i < 10; i++)
	{
		if(modem_dev_dis_detect(dists) == 0)
		{
			int j;	
			for(j = 0; j < MDM_DEV_USONIC_NUM; j++)
			{
				printf("%f, ", dists[j]);
			}
			printf("\n");
		}

		sleep(1);
	}

	modem_dev_close();
	return 0;
}

int modem_dev_open(char *devName)
{
	mdm_dev_fd = open(devName, O_RDWR | O_NOCTTY | O_NONBLOCK); 
	if (mdm_dev_fd < 0) 
	{
		perror("Modem device open error\n"); 
		return -1;
	}

	tcgetattr(mdm_dev_fd,&oldtio); /* save current serial port settings */
	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

	/* 
	BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
	CRTSCTS : output hardware flow control (only used if the cable has
        all necessary lines. See sect. 7 of Serial-HOWTO)
	CS8     : 8n1 (8bit,no parity,1 stopbit)
	CLOCAL  : local connection, no modem contol
	CREAD   : enable receiving characters
	*/
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;  // no CRTSCTS, by hongling.shi@isima.fr

	/*
	IGNPAR  : ignore bytes with parity errors
	ICRNL   : map CR to NL (otherwise a CR input on the other computer
	will not terminate input)
	otherwise make device raw (no other input processing)
	*/
	newtio.c_iflag = IGNPAR | ICRNL;

	/*
	   Raw output.
	 */
	newtio.c_oflag = 0;

	/*
	ICANON  : enable canonical input
	disable all echo functionality, and don't send signals to calling program
	 */
	newtio.c_lflag = 0; // no ICANON;   by hongling.shi@isima.fr

	/* 
	   initialize all control characters 
	   default values can be found in /usr/include/termios.h, and are given
	   in the comments, but we don't need them here
	 */
	newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	newtio.c_cc[VERASE]   = 0;     /* del */
	newtio.c_cc[VKILL]    = 0;     /* @ */
	newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	newtio.c_cc[VSWTC]    = 0;     /* '\0' */
	newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
	newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	newtio.c_cc[VEOL]     = 0;     /* '\0' */
	newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	newtio.c_cc[VEOL2]    = 0;     /* '\0' */

	/* 
	   now clean the modem line and activate the settings for the port
	 */
	tcflush(mdm_dev_fd, TCIFLUSH);
	tcsetattr(mdm_dev_fd, TCSANOW, &newtio);

	return 0;
}

int modem_dev_close()
{
 	/* restore the old port settings */
 	tcsetattr(mdm_dev_fd, TCSANOW, &oldtio);

	close(mdm_dev_fd);
	mdm_dev_fd = -1;

	return 0;
}

int modem_dev_dis_detect(double dis[MDM_DEV_USONIC_NUM])
{
	int res;
	unsigned char RXbuf[256];

	res = read(mdm_dev_fd, RXbuf, 255); 
	if(res < 24)
	{
		return -1;
	}

	int i;
	for(i=0; i<=res-24; i++)
	{
		if((RXbuf[i]==0x35) && (RXbuf[i+1]==0x35))
		{
			//Distance
			int j;
			for(j=0; j<8; j++)
			{
				short temp = (short)((((unsigned short)RXbuf[i+8+j*2+1])<<8) 
					| (RXbuf[i+8+j*2]&0xff));
				temp = (short)(temp * 1600l / 1024l);
				long distance = temp * 254l / 644l;

				dis[j] = distance / 100.0;
			}

			break;
		}
	}
	
	return 0;
}

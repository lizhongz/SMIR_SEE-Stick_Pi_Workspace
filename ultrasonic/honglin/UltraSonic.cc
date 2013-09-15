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

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B38400            
/* change this definition for the correct port */
//#define MODEMDEVICE "/dev/ttyUSB0"
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define TXEn 0
#define RXEn 1

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE; 

main()
{
  int fd,c, res, i, j;
  struct termios oldtio,newtio;
  unsigned char RXbuf[256];
  char TXbuf[] = "From Raspberry Pi\r\nHello\r\n";
/* 
  Open modem device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
*/
 fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK); 
 if (fd <0) {perror(MODEMDEVICE); exit(-1); }

 tcgetattr(fd,&oldtio); /* save current serial port settings */
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
 tcflush(fd, TCIFLUSH);
 tcsetattr(fd,TCSANOW,&newtio);

/*
  terminal settings done, now handle input
  In this example, inputting a 'z' at the beginning of a line will 
  exit the program.
*/
 while (STOP==FALSE) {     /* loop until we have a terminating condition */
 /* read blocks program execution until a line terminating character is 
    input, even if more than 255 chars are input. If the number
    of characters read is smaller than the number of chars available,
    subsequent reads will return the remaining chars. res will be set
    to the actual number of characters actually read */

    if(TXEn)
    {
    	write(fd, TXbuf, strlen(TXbuf));    // by hongling.shi@isima.fr
		sleep(1);
    }

    if(RXEn)
    {
    	res = read(fd,RXbuf,255); 
		if(res >= 24)
		{    	
	//	    RXbuf[res]=0;             /* set end of string, so we can printf */
		    printf("RECE Len=%d\r\n", res);
			for(i=0; i<=res-24; i++)
			{
				if((RXbuf[i]==0x35) && (RXbuf[i+1]==0x35))
				{
					//battery:
					{
						short temp = (short)((((unsigned short)RXbuf[2+1])<<8) | (RXbuf[2]&0xff));
						temp = (short)(temp * 1600l / 1024l);
						temp *= 11;
						temp += 360;	//Adjust for Schottky Rectifiers
						printf("%dmV\t", temp);
					}
					
					//temperature: 
					{
						short temp = (short)((((unsigned short)RXbuf[4+1])<<8) | (RXbuf[4]&0xff));
						printf("%.1fC\t", (temp*0.5));	//* each LSB = 0.5°C   
					}
					
					//Light
					{
						short temp = (short)((((unsigned short)RXbuf[6+1])<<8) | (RXbuf[6]&0xff));
						temp = (short)(temp * 1600l / 1024l / 2 / 2);
						printf("%dlux\t", temp);
					}
					
					//Distance
					for(j=0; j<8; j++)
					{
						short temp = (short)((((unsigned short)RXbuf[i+8+j*2+1])<<8) | (RXbuf[i+8+j*2]&0xff));
						temp = (short)(temp * 1600l / 1024l);
						long distance = temp * 254l / 644l;
						printf("%.2f\t", distance/100.0);
					}
					printf("\r\n");
					i+=17;
				}
			}
		}
		sleep(1);
    }
    // if (RXbuf[0]=='z') STOP=TRUE;
 }
 /* restore the old port settings */
 tcsetattr(fd,TCSANOW,&oldtio);
}


//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//


// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;


// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_READ(g)    (((*(gpio + 13)) & (1<<(g))) != 0)

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GPIO_WHEEL_A 4
#define GPIO_WHEEL_B 17


void setup_io();

#define ST_S1 1
#define ST_S2 3
#define ST_S3 2
#define ST_S4 0

const char* strStateName[4]= {
	"ST_S4",
	"ST_S1",
	"ST_S3",
	"ST_S2"
};

int main(int argc, char **argv)
{ 
  int i;
  int bWheelA, bWheelA_old=3;
  int bWheelB, bWheelB_old=3;
  int nState, nState_old = ST_S3;
  int bDetect;
  int nCount = 0;
  // Set up gpi pointer for direct register access
  setup_io();

 /************************************************************************\
  * You are about to change the GPIO settings of your computer.          *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/

    INP_GPIO(GPIO_WHEEL_A);
    INP_GPIO(GPIO_WHEEL_B);
  
  for (;;)
  {
		bDetect = 0;
       bWheelA = GPIO_READ(GPIO_WHEEL_A);
       bWheelB = GPIO_READ(GPIO_WHEEL_B);
	   nState = (bWheelA + bWheelB*2) % 4;
	   
	   if(bWheelA != bWheelA_old)
	   {
			bWheelA_old = bWheelA;
			bDetect = 1;
		}
	   if(bWheelB != bWheelB_old)
	   {
			bWheelB_old = bWheelB;
			bDetect = 1;
		}
		if(bDetect)
		{
//			printf("CH A B %d %d\n", bWheelA, bWheelB);
			if(    ( (nState==ST_S4) && (nState_old==ST_S3) )
				|| ( (nState==ST_S3) && (nState_old==ST_S2) )
				|| ( (nState==ST_S2) && (nState_old==ST_S1) )
				|| ( (nState==ST_S1) && (nState_old==ST_S4) )
				)
			{
				nCount ++;
				printf("Anti-Clockwise %s=>%s\n", strStateName[nState_old], strStateName[nState]);
			}
			
			else if(    ( (nState==ST_S2) && (nState_old==ST_S3) )
				|| ( (nState==ST_S1) && (nState_old==ST_S2) )
				|| ( (nState==ST_S4) && (nState_old==ST_S1) )
				|| ( (nState==ST_S3) && (nState_old==ST_S4) )
				)
			{
				nCount --;
				printf("Clockwise %s=>%s\n", strStateName[nState_old], strStateName[nState]);
//				printf("Clockwise %d %d\n", nState_old, nState);
			}
			
			else
			{
				printf("Unkown %s=>%s\n", strStateName[nState_old], strStateName[nState]);
//				printf("Unkown Clockwise %d %d\n", nState_old, nState);
			}
			
			printf("nCount=%d\n", nCount);
			
			nState_old = nState;
		}
  }

  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io
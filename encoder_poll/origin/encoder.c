//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//


// Access from ARM Running Linux

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

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g)	*(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)	*(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a)	*(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_READ(g)	(((*(gpio + 13)) & (1<<(g))) != 0)

#define GPIO_SET	*(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR	*(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GPIO_EN_CH_A	9 
#define GPIO_EN_CH_B	10
#define EN_STATE	(GPIO_READ(GPIO_EN_CH_A) + GPIO_READ(GPIO_EN_CH_B) * 2)

void setup_io();

#define EN_ST_S1	1
#define EN_ST_S2	3
#define EN_ST_S3	2
#define EN_ST_S4	0

// Ration diretions
#define EN_DIR_CLK	0	// Clockwise: S1->S2->S3->S4->S1...
#define EN_DIR_ANT_CLK  1	// Anti-clockwise: S4->S3->S2->S1->S4...

int main(int argc, char **argv)
{ 
	int preState;
	int curState;
	int preDir;
	int curDir;
	int ticks;
	int count;

	// Set up gpi pointer for direct register access
	setup_io();

	/************************************************************************\
	 * You are about to change the GPIO settings of your computer.          *
	 * Mess this up and it will stop working!                               *
	 * It might be a good idea to 'sync' before running this program        *
	 * so at least you still have your code changes written to the SD-card! *
	 \************************************************************************/

	// Set the two chanels as input
	INP_GPIO(GPIO_EN_CH_A);
	INP_GPIO(GPIO_EN_CH_B);

	preState = EN_STATE;
	preDir = EN_DIR_CLK;
	ticks = 0;
	count = 0;

	for (;;)
	{
		curState = EN_STATE;

		if( ( (preState == EN_ST_S1) && (curState == EN_ST_S2) )
			|| ( (preState == EN_ST_S2) && (curState == EN_ST_S3) )
			|| ( (preState == EN_ST_S3) && (curState == EN_ST_S4) )
			|| ( (preState == EN_ST_S4) && (curState == EN_ST_S1) ) )
		{
			// Clockwise rotaion
			
			curDir = EN_DIR_CLK;
		}
		else if( ( (preState == EN_ST_S4) && (curState == EN_ST_S3) )
			|| ( (preState == EN_ST_S3) && (curState == EN_ST_S2) )
			|| ( (preState == EN_ST_S2) && (curState == EN_ST_S1) )
			|| ( (preState == EN_ST_S1) && (curState == EN_ST_S4) ) )
		{
			// Anti-clockwise rotation

			curDir = EN_DIR_ANT_CLK;
		}
		else if( curState == preState )
		{
			// No rotation
			
			continue;
		}
		else
		{
			// Unknown state transition

			preState = curState;
			preDir = EN_DIR_CLK;
			ticks = 0;

			continue;
		}

		// Clockwis or anti-clockwise rotation
 
		if( preDir == curDir ) // The direction is the same
		{
			ticks = (ticks + 1) % 4;

			if( ticks == 0 )
			{
				// Passed a window an bar pair
				
				if ( curDir == EN_DIR_CLK ) // Clockwise
					count += 1;
				else // Anti-clockwise
					count -= 1;
	
				printf("Paris: %d\n", count);
			}
		}	
		else // The direction is opposite
		{
			preDir = curDir;
			ticks = 1;
		}

		preState = curState;
	}

	return 0;
}


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


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
#include <pthread.h>
#include "EncoderDevice.h"

/* GPIO's global variables */

static int	mem_fd;
static char	*gpio_mem;
static char	*gpio_map;
static char	*spi0_mem;
static char	*spi0_map;
volatile unsigned	*gpio;	// I/O access 

/* Encoder's gloable variables */

static int	en_dev_running; // 1-running, 0-stopped
static int	en_dev_count;	// The number of window and bar pair
// The ID of the listening thread
static pthread_t	en_dev_lst_thread_ID;	

int main(int argc, char **argv)
{
	int i = 0;

	en_dev_open();

	while(1);
	//pthread_join(en_dev_lst_thread_ID, NULL);

	en_dev_close();

	return 0;
}

int en_dev_open()
{ 
	int	preState;
	int	curState;
	int	preDir;
	int	curDir;
	int	ticks;	// Every four ticks correspons a count

	// Set up gpio pointer for direct register access
	gpio_setup();	

	// Set the two chanels as input
	INP_GPIO(GPIO_EN_CH_A);
	INP_GPIO(GPIO_EN_CH_B);

	// Create a listening thread for counting
	pthread_create(&en_dev_lst_thread_ID, NULL, en_dev_listen, NULL);		
	//en_dev_listen(NULL);

	return 0;
}


int en_dev_close()
{
	// Close the listening thread
	en_dev_running = 0;
	pthread_join(en_dev_lst_thread_ID, NULL);

	return 0;
}

void* en_dev_listen(void * param)
{
	// Initialize variables 
	int	preState;
	int	curState;
	int	preDir;
	int	curDir;
	int	ticks;	// Every four ticks correspons a count

	preState	= EN_STATE;
	preDir		= EN_DIR_CLK;
	ticks		= 0;
	en_dev_count	= 0;
	en_dev_running	= 1;

	// Start to count
	while(en_dev_running)
	{
		curState = EN_STATE; // Obtain encoder's current state

		// Check if there is a state transition
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

			printf("unknow\n");
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
					en_dev_count += 1;
				else // Anti-clockwise
					en_dev_count -= 1;
	
				printf("Paris: %d\n", en_dev_count);
			}
		}	
		else // The direction is opposite
		{
			preDir = curDir;
			ticks = 1;
		}

		preState = curState;
	}

	return NULL;
}

int en_dev_get_cnt_val()
{
	return en_dev_count;
}

int gpio_setup()
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

	return 0;
}

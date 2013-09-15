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
#include "VbrtDev.h"

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_READ(g)    (((*(gpio + 13)) & (1<<(g))) != 0)

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GPIO_VIB 8

int  mem_fd = -1;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;

volatile unsigned *gpio; // I/O access

static int setup_io();

int vbrt_dev_open()
{
	if(setup_io() == -1)
	{
		printf("VbrtDev: GIPO setup error\n");
		return -1;
	}

	// Set Vibration GPIO pin as output
	OUT_GPIO(GPIO_VIB);

	return 0;
}

void vbrt_dev_start()
{
	GPIO_SET = 1 << GPIO_VIB;
}

void vbrt_dev_stop()
{

	GPIO_CLR = 1 << GPIO_VIB;
}

int vbrt_dev_close()
{
	close(mem_fd);
	free(gpio_mem);	
	mem_fd = -1;

	return 0;
}

int setup_io()
{
	/* open /dev/mem */
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
	{
		printf("VbrtDev: can't open /dev/mem \n");
		return -1;
	}

	/* mmap GPIO */

	// Allocate MAP block
	if ((gpio_mem = (char *)malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) 
	{
		printf("VbrtDev: allocation error \n");
		return -1;
	}

	// Make sure pointer is on 4K boundary
	if ((unsigned long)gpio_mem % PAGE_SIZE)
	{
		gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);
	}

	// Now map it
	gpio_map = (char *)mmap(
			(caddr_t)gpio_mem,
			BLOCK_SIZE,
			PROT_READ|PROT_WRITE,
			MAP_SHARED|MAP_FIXED,
			mem_fd,
			GPIO_BASE
			);

	if ((long)gpio_map < 0) 
	{
		printf("VbrtDev: mmap error %d\n", (int)gpio_map);
		return -1;
	}

	// Always use volatile pointer!
	gpio = (volatile unsigned *)gpio_map;

	return 0;
}

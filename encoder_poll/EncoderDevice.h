
#ifndef _ENCODER_DEVICE_H_
#define _ENCODER_DEVICE_H_

/* GPIO Configration */

#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000) // GPIO controller
#define PAGE_SIZE		(4*1024)
#define BLOCK_SIZE		(4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g)		*(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)		*(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a)	*(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_READ(g)		(((*(gpio + 13)) & (1<<(g))) != 0)
#define GPIO_SET		*(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR		*(gpio+10) // clears bits which are 1 ignores bits which are 0

/* Encoder configration */

// Encoder's GPIO pins 
#define GPIO_EN_CH_A	4	// Channel A's pin
#define GPIO_EN_CH_B	17	// Channel B's pin

// Encoder's states 
#define EN_ST_S1	1	// Channel B = 0, channel A = 1
#define EN_ST_S2 	3	// Channel B = 1, channel A = 1
#define EN_ST_S3 	2	// Channel B = 1, channel A = 0
#define EN_ST_S4 	0	// Channel B = 0, channel A = 0

// Ration diretions
#define EN_DIR_CLK	0	// Clockwise: S1->S2->S3->S4->S1...
#define EN_DIR_ANT_CLK  1	// Anti-clockwise: S4->S3->S2->S1->S4...

// Obtain encoder's current state
#define EN_STATE	(GPIO_READ(GPIO_EN_CH_A) + GPIO_READ(GPIO_EN_CH_B) * 2)

/* GPIO functions */

int gpio_setup(); 		// Set up a memory regions to access GPIO

/* Encoder functions */

int en_dev_open();		// Open and enable encoder counting

int en_dev_close();		// Disable and close encoder 

int en_dev_get_count();		// Get the count value

void* en_dev_listen(void *);	// The function for the counting thread


#endif // _ENCODER_DEVICE_H_


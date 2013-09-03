#ifndef _WHEEL_ENCODER_H_
#define _WHEEL_ENCODER_H_

#include <math.h>

#define WH_CPR		150.0	// The number of window and bar pair per revolution
#define WH_RADIUS	10.0	// Unit: millimeter 
#define WH_CIRCUM	(2 * M_PI * WH_RADIUS)	// Circumference of wheel

#define WH_BUF_SIZE	80	// The size of buffer for reading encoder count

// Open encoder device
int wh_open(char *devName);

// Close encoder device
void wh_close();

// Get encoder's count
int wh_get_count(long *pCnt);


#endif // _WHEEL_ENCODER_H_

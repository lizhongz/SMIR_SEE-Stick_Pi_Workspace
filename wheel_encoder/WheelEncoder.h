#ifndef _WHEEL_ENCODER_H_
#define _WHEEL_ENCODER_H_

#include <math.h>

#define WH_CPR		811.0	// The number of window and bar pair per revolution
#define WH_RADIUS	194	// Unit: millimeter 
#define WH_CIRCUM	(2 * M_PI * WH_RADIUS)	// Circumference of wheel

#define WH_DEV_NAME	"/dev/encoder" // Device name

#define WH_BUF_SIZE	80	// The size of buffer for reading encoder count

// Open encoder device
int wh_open();

// Close encoder device
void wh_close();

// Get encoder's count
int wh_get_count(long *pCnt);


#endif // _WHEEL_ENCODER_H_

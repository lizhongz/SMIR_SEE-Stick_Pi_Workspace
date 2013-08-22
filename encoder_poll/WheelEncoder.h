#ifndef _WHEEL_ENCODER_H_
#define _WHEEL_ENCODER_H_

#include <math.h>

/* Wheel parameters */

#define WH_RADIUS	10.0	// Unit: millimeter
#define WH_CIRCUM	(2 * M_PI * WH_RADIUS)	// Circumference

/* Codewheel parameters */

#define WH_CPR		75	// The number of window and bar pair per revolution (CPR)

// Calcultate displacement
#define WH_CALC_DISP(cnt)	(WH_CIRCUM * cnt / WH_CPR)

/* Wheel encoder functions */

// Open wheel encoder's functions
int wh_open();

// Close wheel encoder's functions
int wh_close();

// Calculate current velocity and acceleration
int wh_calc_v_a(float *pV, float *pA);

#endif // _WHEEL_ENCODER_H_


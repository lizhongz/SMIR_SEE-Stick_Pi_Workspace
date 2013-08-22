#ifndef _NAV_COMMON_H_
#define _NAV_COMMON_H_

#define EARTH_ANGULAR_RATE 0.0000727
#define 

typedef struct
{
	double lat;	// latitude
	double lon;	// longitude
	double alt;	// altitude
	double azim;	// azimuth
	double vel_e;	// velocity of east component
	double vel_n;	// velocity of nouth component
}NavPAV; // Position, velocity and azimuth information



#endif // _NAV_COMMON_H_


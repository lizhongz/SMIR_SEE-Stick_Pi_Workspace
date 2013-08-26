#ifndef _NAV_COMMON_H_
#define _NAV_COMMON_H_

#define EAR_ANG_RATE	0.000072921159	// The Earth's rotation angular rate

typedef struct
{
	double lat;	// latitude
	double lon;	// longitude
	double azim;	// azimuth
	double vel_e;	// velocity of east component
	double vel_n;	// velocity of nouth component
}NavPAV; // Position, velocity and azimuth information



#endif // _NAV_COMMON_H_


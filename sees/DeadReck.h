#ifndef _DEAD_RECKONING_H_
#define _DEAD_RECKONING_H_

#include "NavCommon.h"

class DeadReck
{
private:
	NavPAV pav; // positon, azimuth and velocity	

	double altitude;

public:
	DeadReck();

	DeadReck(
		double lat,	// Latitude, unit:radian
		double lon,	// Longitude, unit:radian
		double alt,	// Altitude, unit: meter 
		double azim	// Azimuth, unit: radian
	);

	~DeadReck();
	
	// Reset initial position and azimuth 
	void reset(
		double lat,	// Latitude, unit:radian
		double lon,	// Longitude, unit:radian
		double alt,	// Altitude, unit: meter 
		double azim	// Azimuth, unit: radian
	);

	// Execute DR calculations: update postion, velocity and azimuth
	void operate(
		double vel, 	// Raw velocity from wheel encoder, unit meters/second
		double angRate, // Angular rate from gyro, unit radians/second
		double tItvl,   // Time interval between two sampling, unit seconds
		NavPAV *pPAV	// Updated pav for returning
	); 

	// Suppose object is moving in a plane
	void operate_plane(
		double vel, 	// Raw velocity from wheel encoder, unit meters/second
		double angRate, // Angular rate from gyro, unit radians/second
		double tItvl,   // Time interval between two sampling, unit seconds
		NavPAV *pPAV	// Updated pav for returning
	);

private:
	void update_azimuth(double angRate, double tItvl);
	void update_velocity(double vel);
	void update_position(double tItvl);

};

#endif // _DEAD_RECKONING_H_

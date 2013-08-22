#ifndef _DEAD_RECKONING_H_
#define _DEAD_RECKONING_H_

#include "NavCommon.h"

class DeadReck
{
private:
	NavPAV pav; // positon, azimuth and velocity	

	double altitude;

public:
	DeadReck(double lat, double lon, double alt, double azim);

	~DeadReck();
	
	// Reset initial position and azimuth 
	void reset(double lat, double lon, double alt, double azim);

	// Execute DR calculations: update postion, velocity and azimuth
	void operate(
		double vel, 	// Raw velocity from wheel encoder
		double angRate, // Angular rate from gyro
		double tItvl,   // Time interval between two sampling
		NavPav *pPAV	// Updated pav for returning
	); 

private:
	void update_azimuth();

	void update_velocity();

	void update_position();

};

#endif // _DEAD_RECKONING_H_

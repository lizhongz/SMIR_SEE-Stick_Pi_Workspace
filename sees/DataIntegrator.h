#ifndef _DATA_INTEGRATOR_H_
#define _DATA_INTEGRATOR_H_

#include "NavCommon.h"
#include "Sampler.h"
#include "DeadReck.h"
#include "Runnable.h"

#define SMP_INTERVAL	10000	// Sampling intervel, microsecond
#define CALI_INTERVAL	10	// The calibration interval 
				// is equal to this value multiplying 
				// SMP_INTERVAL

#define WH_CPR		811.0	// The number of window and bar pair per revolution
#define WH_RADIUS	0.194	// Unit: meters
#define WH_CIRCUM	(2 * M_PI * WH_RADIUS)	// Circumference of wheel

class DataIntegrator : public Runnable
{
private:

	NavPAV		gpsPav;		// pav obtained from GPS
	NavPAV		drPav;		// pav calculated by dead reckoning
	NavPAV		optmPav;	// pav calibrated by KF
	Sampler		smpr;
	DeadReck	ddrk;		
	
public:
	DataIntegrator();

	~DataIntegrator();

	int initialize();

	// Start data integration thread
	void run();

	int fuse(); // multiple sensor data
	
	void get_gps_pav(NavPAV &pav);

	void get_dr_pav(NavPAV &pav);

	void get_fused_pav(NavPAV &pav);

};

#endif // _DATA_INTEGRATOR_H_

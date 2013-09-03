#ifndef _DATA_INTEGRATOR_H_
#define _DATA_INTEGRATOR_H_

#include "NavCommon.h"
#include "Sampler.h"
#include "DeadReck.h"

#define SMP_INTERVAL	10000	// Sampling intervel, microsecond

class DataIntegrator
{
private:

	NavPAV		pav;	// Positon, azimuth, velocity
	Sampler		smpr;
	DeadReck	ddrk;		
	
public:
	DataIntegrator();

	~DataIntegrator();

	int initialize();

	int fuse();
	
	void get_fused_data(NavPAV &pav);

};

#endif // _DATA_INTEGRATOR_H_

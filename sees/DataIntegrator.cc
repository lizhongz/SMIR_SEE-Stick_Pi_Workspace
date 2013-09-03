#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include "DataIntegrator.h"
#include "DegRadConv.h"

DataIntegrator::DataIntegrator()
{

}

DataIntegrator::~DataIntegrator()
{
	smpr.close_devices();
}

int DataIntegrator::initialize()
{
	// Open navigation sesors
	smpr.open_devices();
}

int DataIntegrator::fuse()
{
	long	enPreCnt;
	long	enCurCnt;
	int 	distance;
	int	velocity;
	double	angRate;

	struct	timespec tp1;
	struct	timespec tp2;
	double	elapsedTime;

	// Read initial encoder count
	smpr.get_en_count(&enPreCnt, &angRate);

	// Initialize dead reckoning
	ddrk.reset(0, 0, 0, 0);		

	clock_gettime(CLOCK_MONOTONIC, &tp1);
	
	while(1)
	{
		usleep(SMP_INTERVAL);		
		clock_gettime(CLOCK_MONOTONIC, &tp2);

		// Calculate time elapsed time
		elapsedTime = tp2.tv_sec - tp1.tv_sec +
			 (tp2.tv_nsec - tp1.tv_nsec) / 1000000000.0;

		// Sampling
		smpr.get_cnt_rate(&enCurCnt, &angRate);	

		// Calculate velocity
		distance = WH_CIRCUM * ((enCurCnt - enPreCnt) / WH_CPR);
		velocity = distance / (SMP_INTERVAL / 1000000.0);
		// Convert the unit of angRate (degrees to radians)
		angRate = deg2rad(angRate);	

		// Execute dead reckoning
		ddrk.operate(velocity, angRate, elapsedTime, &(this->pav));	

		enPreCnt = enCurCnt;
		tp1 = tp2;
	}
}
	
void DataIntegrator::get_fused_data(NavPAV &pav)
{
	pav = this->pav;
}

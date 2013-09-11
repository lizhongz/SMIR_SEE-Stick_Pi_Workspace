#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include "DataIntegrator.h"
#include "DegRadConv.h"
#include "log.h"

using namespace std;

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
	if(smpr.open_devices() == -1)
	{
		printf("Fail to initialize device\n");
		return -1;
	}
	
	return 0;
}

int DataIntegrator::fuse()
{
	long	enPreCnt;
	long	enCurCnt;
	long	cntDif;
	int 	distance;
	int	velocity;
	double	angRate;
	double	lat;
	double	lon;
	double	alt;
	double	azim;	

	struct	timespec tp1;
	struct	timespec tp2;
	double	elapsedTime;
	int	intvlCnt = 1;

	int	res;

	// Read initial encoder count
	if(smpr.get_en_count(&enPreCnt) != 0)
	{
		return -1;
	}

	// Obtain initial position
	if(smpr.get_gps_3d_pos(&lat, &lon, &alt) != 0)
	{
		return -1;
	}
	
	// Initialize dead reckoning
	ddrk.reset(lat, lon, alt, 0); // Geographic operation
	//ddrk.reset(0, 0, 0, 0); // For plane operation 

	clock_gettime(CLOCK_MONOTONIC, &tp1);
	
	while(1)
	{
		usleep(SMP_INTERVAL);		
		clock_gettime(CLOCK_MONOTONIC, &tp2);

		// Sampling
		smpr.get_cnt_rate(&enCurCnt, &angRate);	

		// Calculate time elapsed time
		elapsedTime = tp2.tv_sec - tp1.tv_sec +
			 (tp2.tv_nsec - tp1.tv_nsec) / 1000000000.0;

		// Calculate velocity
		cntDif = enCurCnt - enPreCnt;	
		if(cntDif < 0)
		{
			cntDif = cntDif / 2;
		}
		distance = WH_CIRCUM * (cntDif / WH_CPR);
		velocity = distance / elapsedTime;

		// Convert the unit of angRate (degrees to radians)
		if(angRate > 0.5 || angRate < -0.5)
		{
			angRate = deg2rad(angRate); // unit radians/s	
		}
		else
		{
			angRate = 0;
		}

		// Execute dead reckoning
			
		//ddrk.operate_plane(velocity, angRate, elapsedTime, &(this->pav));

		ddrk.operate(velocity, angRate, elapsedTime, &(this->drPav));	
		FILE_LOG(logINFO) << "[DR Data] " 
			<< "lat: " << drPav.lat 
			<< ", lon: " << drPav.lon 
			<< ", azi: " << rad2deg(drPav.azim)
			<< ", veN: " << drPav.vel_n
			<< ", veE: " << drPav.vel_e
			<< ", enCnt: " << enCurCnt - enPreCnt
			<< ", vel: " << velocity
			<< ", rate: " << rad2deg(angRate) << endl;

		if(intvlCnt == CALI_INTERVAL)	
		{
			// Calibration 
	
			// Obtain GPS data
			res = smpr.get_gps_pav(&gpsPav.lat, &gpsPav.lon, 
				&gpsPav.azim, &gpsPav.vel);				

			if(res == 0) // If GPS data is valid
			{
				gpsPav.azim = deg2rad(gpsPav.azim);
                                
				FILE_LOG(logINFO) << "[GPS Data] " 
					<< "lat: " << gpsPav.lat
					<< ", lon: " << gpsPav.lon 
					<< ", azi: " << rad2deg(gpsPav.azim)
					<< ", vel: " << gpsPav.vel << endl; 
				
				intvlCnt = 1;	
			}
		}

		enPreCnt = enCurCnt;
		tp1 = tp2;
		intvlCnt += 1;
	}
}
	
void DataIntegrator::get_fused_pav(NavPAV &pav)
{
	pav = this->drPav;
}

void DataIntegrator::get_dr_pav(NavPAV &pav)
{
	pav = this->drPav;
}

void DataIntegrator::get_gps_pav(NavPAV &pav)
{
	pav = this->gpsPav;
}




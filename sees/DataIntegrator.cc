#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <iomanip>
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
	double	cntDif;
	double 	distance;
	double	velocity;
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

	printf("Begin to fuse sensor data\n");

	// Read initial encoder count
	if(smpr.get_en_count(&enPreCnt) != 0)
	{
		cout << "DataIntegrator: fail to get encoder count" 
			<< endl;
		return -1;
	}

	// Obtain initial position
	if(smpr.get_gps_3d_pos(&lat, &lon, &alt) != 0)
	{
		cout << "DataIntegrator: GPS data is not valid" << endl;
		//return -1;
	}
	
	lat = deg2rad(lat);
	lon = deg2rad(lon);

	// Obtain intial azimuth
	if(smpr.get_cmps_azimuth(&azim) != 0)
	{
		cout << "DataIntegrator: fail to get compass azimuth" 
			<< endl;
		return -1;
	}
	
	// Initialize dead reckoning
	ddrk.reset(lat, lon, alt, azim); // Geographic operation
	//ddrk.reset(lat, lon, alt, deg2rad(320)); // Geographic operation
	//ddrk.reset(0, 0, 0, 0); // For plane operation 
	//ddrk.reset(deg2rad(45.759148), deg2rad(3.110976), 412, azim); // ISIMA  Inside
	//ddrk.reset(deg2rad(45.759618), deg2rad(3.111040), 412, deg2rad(335)); // ISIMA-Tram(Poli)
	ddrk.reset(deg2rad(45.759804), deg2rad(3.110615), 412, deg2rad(67.5)); // ISIMA(Amphie)-Tram(Poli)

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
		cntDif = (double)enCurCnt - enPreCnt;	
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
		//printf("lat: %.10f, lon: %.10f\n", rad2deg(drPav.lat), rad2deg(drPav.lon)); 
		/*FILE_LOG(logINFO) << "[DR Data] " 
			<< "lat: " << rad2deg(drPav.lat)
			<< ", lon: " << rad2deg(drPav.lon)
			<< ", azi: " << rad2deg(drPav.azim)
			<< ", veN: " << drPav.vel_n
			<< ", veE: " << drPav.vel_e
			<< ", enCnt: " << enCurCnt - enPreCnt
			<< ", vel: " << velocity
			<< ", rate: " << rad2deg(angRate) << endl;*/
		FILE_LOG(logINFO) << "[DR Data]" << setprecision(10) 
			<< "," << velocity
			<< "," << rad2deg(angRate)
			<< "," << elapsedTime
			<< "," << rad2deg(drPav.lat)
			<< "," << rad2deg(drPav.lon)
			<< "," << rad2deg(drPav.azim)
			<< "," << drPav.vel_n
			<< "," << drPav.vel_e << endl;

		if(intvlCnt++ == CALI_INTERVAL)	
		{
			// Calibration 
	
			// Obtain GPS data

			res = smpr.get_gps_pav(&lat, &lon, &azim, &velocity);				
			if(res == 0) // If GPS data is valid
			{
				gpsPav.lat = deg2rad(lat);
				gpsPav.lon = deg2rad(lon);
				gpsPav.azim = deg2rad(azim);
				gpsPav.vel = velocity;
				gpsPav.vel_e = velocity * sin(gpsPav.azim);
				gpsPav.vel_n = velocity * cos(gpsPav.azim); 
                                
				FILE_LOG(logINFO) << "[GPS Data]" << setprecision(10)
					<< "," << rad2deg(gpsPav.lat)
					<< "," << rad2deg(gpsPav.lon) 
					<< "," << rad2deg(gpsPav.azim)
					<< "," << gpsPav.vel 
					<< "," << gpsPav.vel_n
					<< "," << gpsPav.vel_e << endl;
				
				intvlCnt = 1;	
			}
			else
			{
				FILE_LOG(logINFO) << "[GPS Data],Invalid";
			}

		}

		enPreCnt = enCurCnt;
		tp1 = tp2;
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

void DataIntegrator::run()
{
	m_stop = false;

	cout << "Data integration thread" << endl;

	try
	{
		fuse();
	
	} catch(int i) { /* ... */ }

	m_stop = true;
}


#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "ObstDetection.h"
#include "ModemDev.h"
#include "VbrtDev.h"
#include "NavCommon.h"

using namespace std;

ObstDetection::ObstDetection(DataIntegrator *pDtIntgr)
{
	p_dt_intgr= NULL;

	// Open modem device for ultrasonic sensors
	if(modem_dev_open(MODEM_DEV_NAME) != 0)
	{
		printf("ObstDetection: Modem device open error \n");
		throw -1;
	}

	// Open vibration device
	if(vbrt_dev_open() != 0)
	{
		printf("ObstDetection: Viration device open error\n");		
		throw -1;
	}

	p_dt_intgr= pDtIntgr;
}

ObstDetection::~ObstDetection()
{
	modem_dev_close();
	vbrt_dev_close();
	p_dt_intgr = NULL;
}

void ObstDetection::run()
{
	m_stop = false;

	cout << "Obstacle detection Thread" << endl;

	try
	{
		detect();
	
	} catch(int i) { /* ... */ }

	m_stop = true;
}

int ObstDetection::detect()
{
	double dis[MDM_DEV_USONIC_NUM];		
	double dis2obs;
	int vbrtTime; 
	NavPAV pav;

	while(1)
	{
		stop_point();

		p_dt_intgr->get_fused_pav(pav);
		if(pav.vel < 0.001) // If VIP is not moving
		{
			continue;
		}

		if(modem_dev_dis_detect(dis) == 0)
		{
			// Neglect 0,1,3,4,6,7 ultrasonic sensor
			dis[0] = OBST_NONE;
			dis[2] = OBST_NONE;	
			dis[3] = OBST_NONE;	
			dis[4] = OBST_NONE;	
			dis[6] = OBST_NONE;	
			dis[7] = OBST_NONE;	

			// Find the closest distance to obstacle
			dis2obs = OBST_NONE;
			for(int i = 0; i < MDM_DEV_USONIC_NUM; i++)
			{
				if(dis[i] < dis2obs)
				{
					dis2obs = dis[i];
				}
			}	

			// Send corresponding vibration warning
			if(dis2obs <= OBST_DIS_FAR)
			{
				if(dis2obs <= OBST_DIS_CLOSE)
					vbrtTime = OBST_VBRT_TIME_CLOSE;
				else if(dis2obs <= OBST_DIS_MEDIUM)
					vbrtTime = OBST_VBRT_TIME_MEDIUM;
				else
					vbrtTime = OBST_VBRT_TIME_FAR;
					
				vbrt_dev_start();
				usleep(vbrtTime);
				vbrt_dev_stop();	
			}		
		}

		usleep(OBST_DETC_INTERVAL);
	}

	return 0;
}

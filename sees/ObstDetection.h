#ifndef _OBST_DETECTION_H_
#define _OBST_DETECTION_H_

#include "DataIntegrator.h"
#include "Runnable.h"

#define OBST_DETC_INTERVAL	500000	// micro seconds

// Divide the distance to obstacle
// into three range
#define OBST_DIS_CLOSE	0.5	// 0 ~ 0.5 meters	
#define OBST_DIS_MEDIUM	1.0	// 0.5 ~ 1.0 meters	
#define OBST_DIS_FAR	1.5	// 1.0 ~ 1.5 meters	
#define OBST_NONE	99.9	// No obstacle 	

#define OBST_VBRT_TIME_CLOSE	1000000	// micro seconds	
#define OBST_VBRT_TIME_MEDIUM	500000		
#define OBST_VBRT_TIME_FAR	200000		

#define MODEM_DEV_NAME "/dev/ttyUSB0"

class ObstDetection : public Runnable
{
public:
	
	ObstDetection(DataIntegrator *pDtIntgr);

	~ObstDetection();

	// Start obstacle detection thread
	void run();

	// Detect obstacles
	int detect();

private:

	DataIntegrator *p_dt_intgr;
};

#endif

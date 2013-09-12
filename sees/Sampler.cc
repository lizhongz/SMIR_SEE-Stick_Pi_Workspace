#include <stdlib.h>
#include <stdio.h>
#include "Sampler.h"
#include "GyroDev.h"
#include "WheelEncoder.h"
#include "GPSDevice.h"
#include "GPSMsgParser.h"
#include "CompassDev.h"

Sampler::Sampler()
{

}


Sampler::~Sampler()
{

}

int Sampler::open_devices()
{
	/*if(gps_dev_open((char*)GPS_DEV_FILE_NAME) != 0)
	{
		printf("Open GPS error\n");
		return -1;
	}
	*/

	// Open gyroscope
	if(gyro_dev_open((char*)I2C_DEV_FILE_NAME) != 0)
	{
		printf("Open Gyroscope error\n");
		return -1;
	}

	// Open encoder
	if(wh_open((char*)WH_DEV_FILE_NAME) != 0)
	{
		printf("Open encoder error\n");
		return -1;
	}

	// Open Compass
	if(cmps_dev_open((char*)I2C_DEV_FILE_NAME) != 0)
	{
		printf("Open compass error\n");
		return -1;
	}

	return 0;
}

int Sampler::close_devices()
{
	gps_dev_close();
	gyro_dev_close();
	wh_close();
	cmps_dev_close();

	return 0;
}

int Sampler::get_en_count(long *pCnt)
{
	if(wh_get_count(pCnt) == -1)
	{
		printf("Get encoder count error");
		return -1;
	}

	return 0;
}

int Sampler::get_angular_rate(double *pRate)
{
	if(gyro_dev_get_rate(pRate) == -1)
	{
		printf("Get angular rate error");
		return -1;
	}
	
	return 0;
}

int Sampler::get_cnt_rate(long *pCnt, double *pRate)
{
	if(wh_get_count(pCnt) == -1)
	{
		printf("Get encoder count error");
		return -1;
	}

	if(gyro_dev_get_rate(pRate) == -1)
	{
		printf("Get angular rate error");
		return -1;
	}

	return 0;
}

int Sampler::get_gps_pav(double *lat, double *lon, 
	double *azim, double *vel)
{
	char		msg[GPS_DEV_MSG_SIZE];
	GPSMsgParser	parser;
	RMC_data	rmcData;

	// Get RMC message from GPS
	if(gps_dev_read_rmc(&msg[0], GPS_DEV_MSG_SIZE) == -1)
	{
		return -1;
	}

	printf("%s\n", msg);

	// Parser RMC message
	parser.parse_rmc_msg(string(msg), rmcData);

	*lat	= rmcData.lat;
	*lon	= rmcData.lon;
	*azim	= rmcData.azim;
	*vel	= rmcData.vel;	

	if(rmcData.status == 'V')
	{
		return -1;
	}

	return 0;
}

int Sampler::get_gps_3d_pos(double *lat, double *lon, double *alt)
{
	char		msg[GPS_DEV_MSG_SIZE];
	GPSMsgParser	parser;
	GGA_data	ggaData;

	// Get RMC message from GPS
	if(gps_dev_read_gga(&msg[0], GPS_DEV_MSG_SIZE) == -1)
	{
		return -1;
	}

	printf("%s\n", msg);

	// Parser RMC message
	parser.parse_gga_msg(string(msg), ggaData);

	*lat = ggaData.lat;
	*lon = ggaData.lon;
	*alt = ggaData.alt;

	// If satellite number < 4, dada is not valid
	if(ggaData.sat_num < 4) 
	{
		return -1;
	}

	return 0;
}

int Sampler::get_cmps_azimuth(double *pAzim)
{
	if(cmps_dev_get_azimuth(pAzim) != 0)
	{
		printf("Sampling error: get compass' azimuth\n");	
		return -1;
	}

	return 0;
}


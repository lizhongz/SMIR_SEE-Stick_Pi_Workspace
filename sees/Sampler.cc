#include <stdlib.h>
#include <stdio.h>
#include "Sampler.h"
#include "sensor/GyroDev.h"
#include "sensor/WheelEncoder.h"

Sampler::Sampler()
{

}


Sampler::~Sampler()
{

}

int Sampler::open_devices()
{
	// Open gyroscope
	if(gyro_dev_open(I2C_DEV_FILE_NAME) == -1)
	{
		printf("Open Gyroscope error");
		return -1;
	}

	// Open encoder
	if(wh_open(WH_DEV_FILE_NAME) == -1)
	{
		printf("Open encoder error");
		return -1;
	}

	return 0;
}

int Sampler::close_devices()
{
	gyro_dev_close();
	wh_close();

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


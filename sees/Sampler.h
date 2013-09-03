#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#define I2C_DEV_FILE_NAME	"/dev/i2c-1"	// I2C device file
#define WH_DEV_FILE_NAME	"/dev/encoder"	// Encoder device file

class Sampler
{
public:
	Sampler();

	~Sampler();

	// Open encoder, gyroscope, gps and compass
	int open_devices();
	
	// Close encoder, gyroscope, gps and compass
	int close_devices();

	// Get encoder count from wheel encoder
	int get_en_count(long *pCnt);

	// Get angular rate from gyroscope
	int get_angular_rate(double *pRate);

	// Get encoder count and angular rate
	int get_cnt_rate(long *pCnt, double *pRate);

	// Open encoder, gyroscope, gps and compass
	int open_devices();
	
	// Close encoder, gyroscope, gps and compass
	int close_devices();
};

#endif // _SAMPLER_H_

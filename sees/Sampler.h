#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#define I2C_DEV_FILE_NAME	"/dev/i2c-1"	// I2C device file
#define WH_DEV_FILE_NAME	"/dev/encoder"	// Encoder device file
#define GPS_DEV_FILE_NAME	"/dev/ttyAMA0"	// GPS device file

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

	// Get position, azimuth in degrees, velocity from GPS
	// if data valid return 0, else return -1
	int get_gps_pav(double *lat, double *lon, 
		double *azim, double *vel);

	// Get latitude, longitude and altitude
	// if data valid return 0, else return -1
	int get_gps_3d_pos(double *lat, double *lon, double *alt);
};

#endif // _SAMPLER_H_

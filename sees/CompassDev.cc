#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "CompassDev.h"

static int cmps_dev_fd = -1;

int cmps_dev_open(const char *devName)
{
	int32_t res;

	if ((cmps_dev_fd = open(devName, O_RDWR)) < 0) 
	{
		/* ERROR HANDLING: you can check errno to see what went wrong */
		printf("Failed to open the i2c bus\n");
		return -1;
	}	

	if (ioctl(cmps_dev_fd, I2C_SLAVE, LSM303_ADDR) < 0) 
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		return -1;
	}	
	
	// Initialize magnetometer 
	res = i2c_smbus_write_byte_data(cmps_dev_fd, 
		LSM303_CRA_REG, 0x10); // Output rate 15Hz 
	res = i2c_smbus_write_byte_data(cmps_dev_fd, 
		LSM303_CRB_REG, 0x20); // Input field range +-1.3 Gauss
	res = i2c_smbus_write_byte_data(cmps_dev_fd, 
		LSM303_MR_REG, 0x00);  // Continuous-conversion mode

	return 0;
}

int cmps_dev_close()
{
	close(cmps_dev_fd);
	cmps_dev_fd = -1;	

	return 0;
}

int cmps_dev_get_azimuth(double *pAzim)
{
	unsigned char block[6];
	int16_t xValRaw;	// unit: digits
	int16_t yValRaw;
	int16_t zValRaw;
	double	xVal;		// unit: gauss
	double	yVal;
	double	zVal;
	double	azimuth;
	
	i2c_smbus_read_i2c_block_data(cmps_dev_fd, 
		0x80 | LSM303_OUT_X_H_M, 6, &block[0]);

	xValRaw = (int16_t)((block[0] << 8) + block[1]);
	zValRaw = (int16_t)((block[2] << 8) + block[3]);
	yValRaw = (int16_t)((block[4] << 8) + block[5]);
	
	//xVal = (double)xValRaw / LSM303_GAIN;
	//yVal = (double)yValRaw / LSM303_GAIN;
	//zVal = (double)zValRaw / LSM303_GAIN;

	azimuth = atan2(yValRaw, xValRaw);
	if(azimuth < 0)
		azimuth = azimuth + 2 * M_PI;
	if(azimuth > 2 * M_PI)
		azimuth = azimuth - 2 * M_PI;

	*pAzim = azimuth;

	return 0;
}

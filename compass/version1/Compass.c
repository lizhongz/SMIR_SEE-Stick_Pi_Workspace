/* Magnetometer LSM303DLHC Testing */

#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "Compass.h"

#define LSM303_ADDR 0x1e	
#define LSM303_GAIN 1100	// Input field range: +-1.3

int main()
{
	int32_t	file;
	char	*filename = "/dev/i2c-1";
	int32_t res;
	unsigned char	block[6];
	int16_t xValRaw;	// unit: digits
	int16_t yValRaw;
	int16_t zValRaw;
	double	xVal;		// unit: gauss
	double	yVal;
	double	zVal;
	double	azimuth;

	if ((file = open(filename, O_RDWR)) < 0) 
	{
		/* ERROR HANDLING: you can check errno to see what went wrong */
		perror("Failed to open the i2c bus");
		exit(1);
	}	

	if (ioctl(file, I2C_SLAVE, LSM303_ADDR) < 0) 
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}	
	
	// Initialize magnetometer 
	res = i2c_smbus_write_byte_data(file, 
		LSM303_CRA_REG, 0x10); // Output rate 15Hz 
	res = i2c_smbus_write_byte_data(file, 
		LSM303_CRB_REG, 0x20); // Input field range +-1.3 Gauss
	res = i2c_smbus_write_byte_data(file, 
		LSM303_MR_REG, 0x00);  // Continuous-conversion mode

	while(1)
	{
		i2c_smbus_read_i2c_block_data(file, 0x80 | LSM303_OUT_X_H_M, 6, &block[0]);

		xValRaw = (int16_t)((block[0] << 8) + block[1]);
		zValRaw = (int16_t)((block[2] << 8) + block[3]);
		yValRaw = (int16_t)((block[4] << 8) + block[5]);
		
/*		xVal = (double)xValRaw / LSM303_GAIN;
		yVal = (double)yValRaw / LSM303_GAIN;
		zVal = (double)zValRaw / LSM303_GAIN;
*/
		printf("x: %d, y: %d, z: %d\n", xValRaw, yValRaw, zValRaw);

		azimuth = atan2(yValRaw, xValRaw);
		if(azimuth < 0)
			azimuth = azimuth + 2 * M_PI;
		if(azimuth > 2 * M_PI)
			azimuth = azimuth - 2 * M_PI;

		//printf("azimuth: %f\n", azimuth); 
		printf("azimuth: %f\n", azimuth * 180 / M_PI); 

		usleep(1000000);
	}

	return 0;
}

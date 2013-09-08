#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "GyroDev.h"

static int gyro_dev_fd = -1;

int gyro_dev_open(char *devName)
{
	// Open I2C device file
	if ((gyro_dev_fd = open(devName, O_RDWR)) < 0) 
	{
		perror("Gyro: Failed to open the i2c bus");
		return -1;
	}	

	// Set gyroscope as i2c slave device 
	if (ioctl(gyro_dev_fd, I2C_SLAVE, GYRO_DEV_ADDR) < 0) 
	{
		return -1;
	}	

	// Initialize gyroscope

	i2c_smbus_write_byte_data(gyro_dev_fd, GYRO_CTRL_REG1_G, 
		0x5f); // enable x, y, z axis
	i2c_smbus_write_byte_data(gyro_dev_fd, GYRO_CTRL_REG4_G, 
		0x30); // full scale 2000 dps

	return 0;
}

int gyro_dev_close()
{
	close(gyro_dev_fd);	
	gyro_dev_fd = -1;
}

int gyro_dev_get_rate(double *pRate)
{
	short	zAngRateRaw;
	
	// Read z axis' raw angular rate
	zAngRateRaw = (short)i2c_smbus_read_word_data(
		gyro_dev_fd, GYRO_OUT_Z_L_G | 0x80); 
	// Convert the unit of angular rate from dight 
	// to degrees per second
	*pRate = (double)zAngRateRaw * GYRO_SENS / 1000 - GYRO_ZERO_RATE;

	return 0;
}



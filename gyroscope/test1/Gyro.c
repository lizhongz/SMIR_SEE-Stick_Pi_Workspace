#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define GYRO_DEV_ADDR		0x6a
#define GYRO_CTRL_REG1_G	0x20	
#define GYRO_CTRL_REG4_G	0x23
#define GYRO_OUT_X_L_G		0x28
#define GYRO_OUT_Y_L_G		0x2a
#define GYRO_OUT_Z_L_G		0x2c	// Yaw axix 
#define GYRO_SENS		70.0 	// sensitivity, unit: mdps per digit

#define INTERVAL		10000	// microsecond

int main()
{
	int file;
	char *filename = "/dev/i2c-1";
	int res;
	char buf[2];
	short xAngRateRaw;	// unit: mdps/dight
	short yAngRateRaw;
	short zAngRateRaw;
	double xAngRate;	// unit: dps
	double yAngRate;
	double zAngRate;
	double yawAngle = 0;
	struct timespec tp1;
	struct timespec tp2;
	double elapsedTime;	// second

	if ((file = open(filename, O_RDWR)) < 0) 
	{
		/* ERROR HANDLING: you can check errno to see what went wrong */
		perror("Failed to open the i2c bus");
		exit(1);
	}	

	if (ioctl(file, I2C_SLAVE, GYRO_DEV_ADDR) < 0) 
	{
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}	

	// Initialize gyroscope
	res = i2c_smbus_write_byte_data(file, 
		GYRO_CTRL_REG1_G, 0x5f); // enable x, y, z axis
	res = i2c_smbus_write_byte_data(file, 
		GYRO_CTRL_REG4_G, 0x30); // full scale 2000 dps

	clock_gettime(CLOCK_MONOTONIC, &tp1);
	
	while(1)
	{

		usleep(INTERVAL);	

		// Calculate time interval
		clock_gettime(CLOCK_MONOTONIC, &tp2);
		elapsedTime = tp2.tv_sec - tp1.tv_sec + (tp2.tv_nsec - tp1.tv_nsec) / 1000000000.0;
		//printf("s: %d, ns: %d\n", tp2.tv_sec - tp1.tv_sec, tp2.tv_nsec - tp1.tv_nsec);	

		/*
		xAngRateRaw = (short)i2c_smbus_read_word_data(file, 
			GYRO_OUT_X_L_G | 0x80); // Read x axis' angular rate 
		xAngRate = (double)xAngRateRaw * GYRO_SENS / 1000;

		yAngRateRaw = (short)i2c_smbus_read_word_data(file, 
			GYRO_OUT_Y_L_G | 0x80); // Read y axis' angular rate 
		yAngRate = (double)yAngRateRaw * GYRO_SENS / 1000; 
		*/

		zAngRateRaw = (short)i2c_smbus_read_word_data(file, 
			GYRO_OUT_Z_L_G | 0x80); // Read z axis' angular rate
		zAngRate = (double)zAngRateRaw * GYRO_SENS / 1000;

		//printf("x %.6f, y %.6f, z %.6f\n", xAngRate, yAngRate, zAngRate);
		//printf("x %x, y %x, z %x\n", xAngRateRaw, yAngRateRaw, zAngRateRaw);

		//if(zAngRate > 1 || zAngRate < -3)
		{
			//yawAngle += zAngRate * ((double)INTERVAL / 1000000);
			yawAngle += zAngRate * elapsedTime;
			printf("yaw angle: %f\n", yawAngle);
		}

		tp1.tv_sec = tp2.tv_sec;
		tp1.tv_nsec = tp2.tv_nsec;
	}

	return 0;
}

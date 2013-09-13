/* Gyroscope LSM330DLC driver */

#ifndef _GYRO_DEV_H_
#define _GYRO_DEV_H_ 

#define GYRO_DEV_ADDR		0x6a	// Gyroscope I2C bus address
#define GYRO_CTRL_REG1_G	0x20	
#define GYRO_CTRL_REG4_G	0x23
#define GYRO_OUT_X_L_G		0x28
#define GYRO_OUT_Y_L_G		0x2a
#define GYRO_OUT_Z_L_G		0x2c	// Yaw axix 

#define GYRO_SENS		70.0 	// sensitivity, unit: mdps per digit

#define GYRO_ZERO_RATE		(-1.22)	// Zero-rate level

int gyro_dev_open(char *devName);

int gyro_dev_close();

// Obtain yaw angular rate (unit dps) 
int gyro_dev_get_rate(double *pRate);

#endif // _GYRO_DEV_H_

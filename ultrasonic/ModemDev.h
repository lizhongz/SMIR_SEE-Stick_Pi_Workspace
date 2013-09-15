#ifndef _ULTRASONIC_DEV_H_
#define _ULTRASONIC_DEV_H_

#define MDM_DEV_USONIC_NUM	8 // number of ultrasonic sensors
#define BAUDRATE		B38400            
#define _POSIX_SOURCE		1 // POSIX compliant source


int modem_dev_open(char *devName);

int modem_dev_close();

int modem_dev_dis_detect(double dis[MDM_DEV_USONIC_NUM]);

#endif // _ULTRASONIC_H_

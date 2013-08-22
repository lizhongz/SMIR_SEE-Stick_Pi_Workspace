#ifndef _GPS_DEVICE_H_
#define _GPS_DEVICE_H_

#include <fcntl.h>

#define GPS_DEV_FILE	"/dev/ttyAMA0"
#define GPS_DEV_O_FLAGS	(O_RDONLY | O_NOCTTY | O_NDELAY)
#define GPS_DEV_MSG_SIZE 75 


int gps_dev_open();

void gps_dev_close();

int gps_dev_read(char *msg, int size);

int gps_dev_read_rmc(char *msg, int size);

#endif

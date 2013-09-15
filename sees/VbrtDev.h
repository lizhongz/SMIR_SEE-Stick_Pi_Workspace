#ifndef _VBRT_DEV_H_
#define _VBRT_DEV_H_

// Open vibration device
int vbrt_dev_open();

// Start to vibrate
void vbrt_dev_start();

// Stop vibrating
void vbrt_dev_stop();

// Close vibration device
int vbrt_dev_close();

#endif // _VBRT_DEV_H_

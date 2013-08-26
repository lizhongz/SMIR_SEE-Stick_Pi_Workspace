#include <stdlib.h>
#include <stdio.h>
#include "DeadReck.h"
#include "DegRadConv.h"

int main()
{
	double initLat = deg2rad(0);
	double initLon = deg2rad(0);
	double initAlt = 0;
	double initAzim = deg2rad(45);
	double vel;
	double angRate;
	double tIntv;
	NavPAV pav;

	DeadReck dr(initLat, initLon, initAlt, initAzim);

	// 1st moving: 100 m/s, 0 degree/s, 5 seconds
	vel = 100;
	angRate = deg2rad(0);
	tIntv = 5;
	dr.operate(vel, angRate, tIntv, &pav);
	printf("lat: %f, lon: %f, azim: %f, vel_e: %f m/s, vel_n: %f m/s \n", 
		rad2deg(pav.lat), rad2deg(pav.lon), 
		rad2deg(pav.azim), pav.vel_e, pav.vel_n);

	// 1st moving: 100 m/s, 22.5 degree/s, 2 seconds
	vel = 200;
	angRate = deg2rad(-22.5);
	tIntv = 2;
	dr.operate(vel, angRate, tIntv, &pav);
	printf("lat: %f, lon: %f, azim: %f, vel_e: %f m/s, vel_n: %f m/s \n", 
		rad2deg(pav.lat), rad2deg(pav.lon), 
		rad2deg(pav.azim), pav.vel_e, pav.vel_n);
	
	// 1st moving: 100 m/s, 9 degree/s, 5 seconds
	vel = 100;
	angRate = deg2rad(-9);
	tIntv = 5;
	dr.operate(vel, angRate, tIntv, &pav);
	printf("lat: %f, lon: %f, azim: %f, vel_e: %f m/s, vel_n: %f m/s \n", 
		rad2deg(pav.lat), rad2deg(pav.lon), 
		rad2deg(pav.azim), pav.vel_e, pav.vel_n);
	
	return 0;
}

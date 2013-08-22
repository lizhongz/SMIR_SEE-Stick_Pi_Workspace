/* Geodetic equations */
#ifndef _GEO_EQUATIONS_H_
#define _GEO_EQUATIONS_H_

#include <math.h>

#define ELLIPSOID_A	6378137.0	// Semi-major axis
#define ELLIPSOID_E2	0.0066943800229	// Eccentricity power 2

// Calculate normal radius(meters) of curvature of the Earth's ellipsoid
double calc_normal_radius(double latitude);
// Calculate meridian radius(meters) of curvature of the Earth's ellipsoid
double calc_meridian_radius(double latitude);

// Convert degrees to radian
double deg2rad(double deg);
// Convert radian to degrees
double rad2deg(double rad);


double calc_normal_radius(double latitude)
{
	double latRad = deg2rad(latitude);

	double tmp = (1.0 - ELLIPSOID_E2 * pow(sin(latRad), 2.0));
	return ELLIPSOID_A / sqrt(tmp);
}

double calc_meridian_radius(double latitude)
{
	double latRad = deg2rad(latitude);

	double tmp = (1.0 - ELLIPSOID_E2 * pow(sin(latRad), 2.0));
	return ELLIPSOID_A * (1.0 - ELLIPSOID_E2) / (sqrt(tmp) * tmp);
}

double deg2rad(double deg)
{
	return deg * (M_PI / 180.0);
}

double rad2deg(double rad)
{
	return rad * (180.0 / M_PI);
}

#endif

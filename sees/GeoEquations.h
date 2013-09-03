/* Geodetic equations */
#ifndef _GEO_EQUATIONS_H_
#define _GEO_EQUATIONS_H_

#define ELLIPSOID_A	6378137.0	// Semi-major axis
#define ELLIPSOID_E2	0.0066943800229	// Eccentricity power 2

// Calculate normal radius(meters) of curvature of the Earth's ellipsoid
double calc_normal_radius(
	double lat	// latitude, unit: radian
);
// Calculate meridian radius(meters) of curvature of the Earth's ellipsoid
double calc_meridian_radius(
	double lat	// latitude, unit: radian
);

#endif

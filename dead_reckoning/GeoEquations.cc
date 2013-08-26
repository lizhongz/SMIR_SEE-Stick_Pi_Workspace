#include <math.h>
#include "GeoEquations.h"

double calc_normal_radius(double lat)
{
	double tmp = (1.0 - ELLIPSOID_E2 * pow(sin(lat), 2.0));
	return ELLIPSOID_A / sqrt(tmp);
}

double calc_meridian_radius(double lat)
{
	double tmp = (1.0 - ELLIPSOID_E2 * pow(sin(lat), 2.0));
	return ELLIPSOID_A * (1.0 - ELLIPSOID_E2) / (sqrt(tmp) * tmp);
}


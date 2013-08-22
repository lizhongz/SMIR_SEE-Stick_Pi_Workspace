#include <stdlib.h>
#include <stdio.h>
#include "GeoEquations.h"

int main()
{
	double lat = 43.262858;//40.256667;
	double Rn;
	double Rm;

	Rn = calc_normal_radius(lat);
	Rm = calc_meridian_radius(lat);

	printf("Rn: %f\n Rm: %f\n", Rn, Rm);

	return 0;
}

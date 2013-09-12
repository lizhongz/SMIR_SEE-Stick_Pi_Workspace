#include <stdlib.h>
#include <math.h>
#include "DeadReck.h"
#include "NavCommon.h"
#include "GeoEquations.h"

DeadReck::DeadReck(double lat, double lon, double alt, double azim)
{
	pav.lat = lat;
	pav.lon = lon;
	pav.azim = azim;
	altitude = alt;
}

DeadReck::DeadReck()
{

}

DeadReck::~DeadReck()
{

}
	
void DeadReck::reset(double lat, double lon, double alt, double azim)
{
	pav.lat = lat;
	pav.lon = lon;
	pav.azim = azim;
	altitude = alt;
}

void DeadReck::operate(double vel, double angRate, double tItvl, NavPAV *pPAV)
{
	// Update order: azimuth->velocity->position
	// this order should not be changed 
	update_azimuth(angRate, tItvl);
	update_velocity(vel);	
	update_position(tItvl);

	pPAV->lat = pav.lat;
	pPAV->lon = pav.lon;	
	pPAV->azim = pav.azim;	
	pPAV->vel = pav.vel;
	pPAV->vel_n = pav.vel_n;	
	pPAV->vel_e = pav.vel_e;	
}

void DeadReck::update_azimuth(double angRate, double tItvl)
{
	double azimRate;
	double rn = calc_normal_radius(pav.lat); // Normal radius

	// Calculate the changing rate of the azimuth
	// in the local-level frame	
	azimRate = angRate - (EAR_ANG_RATE * sin(pav.lat) 
		- (pav.vel_e * tan(pav.lat) / (rn + this->altitude)));

	// update azimuth
	pav.azim = pav.azim - azimRate * tItvl;		

	// pav.azim should between -2*Pi and 2*Pi
	double pi2 = 2 * M_PI;
	while(pav.azim < 0)
	{
		pav.azim += pi2;
	}	
	while(pav.azim > pi2)
	{
		pav.azim -= pi2;
	}	
}

void DeadReck::update_velocity(double vel)
{
	pav.vel	= vel;
	pav.vel_e = vel * sin(pav.azim);	
	pav.vel_n = vel * cos(pav.azim);
}

void DeadReck::update_position(double tItvl)
{
	double latRate;
 	double lonRate;
	double rn = calc_normal_radius(pav.lat);   // Normal radius
	double rm = calc_meridian_radius(pav.lat); // Meridian radius	

	// Calculate the changing rate of latitude
	latRate = (1 / (rm + this->altitude)) * pav.vel_n;
	// Calculate the changing rate of longitude	
	lonRate = (1 / ((rn + this->altitude) * cos(pav.lat))) * pav.vel_e;

	// Update latitude
	pav.lat = pav.lat + latRate * tItvl;
	// Update longitude
	pav.lon = pav.lon + lonRate * tItvl;
}

void DeadReck::operate_plane(double vel, double angRate, double tItvl, NavPAV *pPAV)
{
	pav.azim = pav.azim - angRate * tItvl;		
	pav.vel = vel; 
	pav.vel_e = vel * sin(pav.azim);	
	pav.vel_n = vel * cos(pav.azim);
	pav.lat = pav.lat + pav.vel_n * tItvl;
	pav.lon = pav.lon + pav.vel_e * tItvl;

	pPAV->lat = pav.lat;
	pPAV->lon = pav.lon;	
	pPAV->azim = pav.azim;	
	pPAV->vel = vel;
	pPAV->vel_n = pav.vel_n;	
	pPAV->vel_e = pav.vel_e;	
}



#include "DeadReck.h"

DeadReck::DeadReck(double lat, double lon, double alt, double azim)
{
	pav.lat = lat;
	pav.lon = lon;
	pav.azim = azim;
	altitude = alt;
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

void DeadReck::operate(double vel, double angRate, double tItvl, NavPav *pPAV)
{
	pav.azim = update_azimuth(angRate, tItvl);
	
}

void DeadReck::update_azimuth(double angRate, double tItvl)
{
	
}

void DeadReck::update_velocity()
{

}

void DeadReck::update_position()
{

}

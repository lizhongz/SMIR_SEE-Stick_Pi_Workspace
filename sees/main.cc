#include <iostream>
#include "DataIntegrator.h"
#include "Sampler.h"
#include "NetComm.h"
#include "NavCommon.h"
#include "log.h"
#include "ObstDetection.h"

using namespace std;

int main()
{
	// Open log function
	FILE *logFile = fopen("app.log", "w");
	Output2FILE::Stream() = logFile;
        FILELog::ReportingLevel() = FILELog::FromString("DEBUG1");

	// Create data integration thread
	DataIntegrator dtIntr;
	if(dtIntr.initialize() != 0)
	{
		cout << "Error:DataIntegrator initalization" << endl;
		return -1;
	}
	dtIntr.start();

	// Start obstacle detection thread
	ObstDetection obsDect(&dtIntr);
	obsDect.start();

	// Start remote monitoring thread
/*
	NetComm netComm(&dtIntr);
	netComm.start();
*/

	while(1);

/*
	// Test GPS sampling 
	Sampler smplr;
	double lat, lon, alt, azim, vel;

	smplr.open_devices();	
	
	for(int i = 0; i < 3; i++)
	{
		smplr.get_gps_pav(&lat, &lon, &azim, &vel);

		cout << "lat: " << lat
			<< " lon: " << lon
			<< " azim: " << azim
			<< " vel: " << vel << endl;
	}
	cout << endl << endl;

	for(int i = 0; i < 3; i++)
	{
		smplr.get_gps_3d_pos(&lat, &lon, &alt);

		cout << "lat: " << lat
			<< " lon: " << lon
			<< " alt: " << alt << endl;
	}
*/
	
	return 0;
}

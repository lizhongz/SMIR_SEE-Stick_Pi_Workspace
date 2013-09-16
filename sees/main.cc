#include <iostream>
#include "DataIntegrator.h"
#include "Sampler.h"
#include "NetComm.h"
#include "NavCommon.h"
#include "log.h"
#include "ObstDetection.h"

using namespace std;

#define EN_DATA_FUSION		1
#define EN_OTACLE_DETEC		EN_DATA_FUSION
#define EN_NET_COMM		0

int main()
{
	// Open log function
	FILE *logFile = fopen("app.log", "w");
	Output2FILE::Stream() = logFile;
        FILELog::ReportingLevel() = FILELog::FromString("DEBUG1");

#if EN_DATA_FUSION
	// Create data integration thread
	DataIntegrator dtIntr;
	if(dtIntr.initialize() != 0)
	{
		cout << "Error:DataIntegrator initalization" << endl;
		return -1;
	}
	dtIntr.start();
#endif

#if EN_OTACLE_DETCT
	// Start obstacle detection thread
	ObstDetection obsDect(&dtIntr);
	obsDect.start();
#endif

#if EN_NET_COMM
	// Start remote monitoring thread
	NetComm netComm(&dtIntr);
	netComm.start();
#endif

	while(1);

	return 0;
}

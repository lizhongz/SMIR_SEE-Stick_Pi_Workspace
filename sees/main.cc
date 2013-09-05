#include <stdlib.h>
#include "DataIntegrator.h"
#include "log.h"

int main()
{
	DataIntegrator dtIntr;

	// Open log function
	FILE *logFile = fopen("app.log", "w");
	Output2FILE::Stream() = logFile;
        FILELog::ReportingLevel() = FILELog::FromString("DEBUG1");
	
	dtIntr.initialize();
	dtIntr.fuse();	

	return 0;
}

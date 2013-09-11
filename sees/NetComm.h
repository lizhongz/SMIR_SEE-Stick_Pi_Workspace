#ifndef _NET_COMM_H_
#define _NET_COMM_H_

#include <unistd.h>
#include <curl/curl.h>
#include "Runnable.h"
#include "DataIntegrator.h"
#include "NavCommon.h"

#define	GSERVER_URL_HEAD "http://edss.isima.fr/sites/smir/sees/gate/"

#define TRK_PNT_GPS_TYPE	2 
#define TRK_PNT_DR_TYPE		1 
#define TRK_PNT_OPTM_TYPE	2 

class NetComm : public Runnable
{
private:
	DataIntegrator *p_dt_intgr;
	CURL *p_curl;

public:
	NetComm(DataIntegrator *pDtIntgr);

	~NetComm();

	// Start monitor thread
	void run();

	// Get VIP's track points and send them 
	// to global server
	int monitor();
		
	// Send VIP's track points to global server
	int send_track_point(NavPAV *pGpsPav, NavPAV *pDrPav, NavPAV *pOptmPav);

};


#endif // _NET_COMM_H_

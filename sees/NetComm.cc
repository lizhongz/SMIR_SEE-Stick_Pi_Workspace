#include <unistd.h>
#include <string>
#include <curl/curl.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include "NetComm.h"

using namespace std;

size_t curl_callback(void *ptr, size_t size, size_t count, void *stream);

NetComm::NetComm(DataIntegrator *pDtIntgr)
{
	// Initalize http curl object
	p_curl = curl_easy_init();
	curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt(p_curl, CURLOPT_WRITEDATA, NULL);

	p_dt_intgr = pDtIntgr;	
}

NetComm::~NetComm()
{
	curl_easy_cleanup(p_curl);
}

void NetComm::run()
{
	m_stop = false;

	try
	{
		monitor();
	
	} catch(int i) { /* ... */ }

	m_stop = true;
}

int NetComm::monitor()
{
	NavPAV gpsPav;
	NavPAV drPav;
	NavPAV optmPav;
	
	while(1)
	{
		stop_point();

		// Get current track points given by
		// GPS, dead reckoning and data fusion method
		p_dt_intgr->get_gps_pav(gpsPav);
		p_dt_intgr->get_dr_pav(drPav);
		//get_fused_pav(optmPav);

		// Send track points to global server
		send_track_point(&gpsPav, &drPav, NULL);

		sleep(1);
	}

	return 0;
}

int NetComm::send_track_point(NavPAV *pGpsPav, 
	NavPAV *pDrPav, NavPAV *pOptmPav)
{
	ostringstream url;
	ostringstream timeStrm;
	CURLcode res;

	time_t	now = time(0);
	tm *ltm = localtime(&now);
	timeStrm << 1900 + ltm->tm_year << "-" << ltm->tm_mon << "-"
		<< ltm->tm_mday << "%20" << ltm->tm_hour << ":" 
		<< ltm->tm_min << ":" << ltm->tm_sec;
	
	if(pGpsPav != NULL)
	{
		// Send GPS track point

		url << GSERVER_URL_HEAD 
			<< "trackinsert?" 
			<< "nav_id=" << 1 
			<< "&trk_type=" << TRK_PNT_GPS_TYPE
			<< "&trk_time=" << timeStrm.str()
			<< "&trk_lat=" << pGpsPav->lat 
			<< "&trk_long=" << pGpsPav->lon
			<< "&trk_azimuth=" << pGpsPav->azim
			<< "&trk_velocity=" << pGpsPav->vel;

		curl_easy_setopt(p_curl, CURLOPT_URL, url.str().c_str());		
		res = curl_easy_perform(p_curl);
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			return -1;
		}
	}
       	
	if(pDrPav != NULL)
	{
		// Send DR track point

		url.str("");
		url << GSERVER_URL_HEAD 
			<< "trackinsert?" 
			<< "nav_id=" << 1 
			<< "&trk_type=" << TRK_PNT_DR_TYPE
			<< "&trk_time=" << timeStrm.str()
			<< "&trk_lat=" << pDrPav->lat 
			<< "&trk_long=" << pDrPav->lon
			<< "&trk_azimuth=" << pDrPav->azim
			<< "&trk_velocity=" << pDrPav->vel;

		curl_easy_setopt(p_curl, CURLOPT_URL, url.str().c_str());		
		res = curl_easy_perform(p_curl);
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			return -1;
		}
	}

	return 0;
}

size_t curl_callback(void *ptr, size_t size, size_t count, void *stream)
{
	//printf("%s\n", (char*)ptr);	

	return count;
}



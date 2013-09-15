#include <cstdlib>
#include <string>
#include "GPSMsgParser.h"

int GPSMsgParser::parse_rmc_msg(const string &rawMsg, RMC_data &rmcData)
{
	vector<string> splMsgs;
	char delim = ',';

	// Split raw message by ','
	split(rawMsg, delim, splMsgs);

	// convert splitted raw message to RMC data
	if(splMsgs[3].compare("") == 0)
	{
		return -1;
	}

	rmcData.utc	= splMsgs[1];	
	rmcData.status	= (char)splMsgs[2][0];
	rmcData.lat	= atof(splMsgs[3].c_str());
	rmcData.lon	= atof(splMsgs[5].c_str());
	coor_format_convert(rmcData.lat, splMsgs[4][0], 
		rmcData.lon, splMsgs[6][0]);

	rmcData.vel	= atof(splMsgs[7].c_str()) * 0.514444; // knots to m/s
	rmcData.azim	= atof(splMsgs[8].c_str());

	return 0;
}

int GPSMsgParser::parse_gga_msg(const string &rawMsg, GGA_data &ggaData)
{
	vector<string> splMsgs;
	char delim = ',';

	// Split raw message by ','
	split(rawMsg, delim, splMsgs);

	if(splMsgs[2].compare("") == 0)
	{
		return -1;
	}

	ggaData.utc	= splMsgs[1];	
	ggaData.lat	= atof(splMsgs[2].c_str());
	ggaData.lon	= atof(splMsgs[4].c_str());
	ggaData.alt	= atof(splMsgs[9].c_str());
	ggaData.sat_num = atoi(splMsgs[7].c_str());

	coor_format_convert(ggaData.lat, splMsgs[3][0], 
		ggaData.lon, splMsgs[5][0]);

	return 0;
}

void GPSMsgParser::split(const string &rawMsg, const char delim, vector<string> &splMsgs)
{
	string tmpStr("");

	splMsgs.clear();

	// Splilt raw messege into array by delim
	
	for(int i = 0; i < rawMsg.size(); i++)
	{	
		// If not delimiter
		if(rawMsg[i] != delim)
		{
			// If not '\005'(Enquiry character)
			if(rawMsg[i] != '\005')
			{
				tmpStr.push_back(rawMsg[i]);
			}
		}
		else
		{
			if(tmpStr.size() != 0)
			{
				splMsgs.push_back(tmpStr);
				tmpStr.clear();
			}
			else
			{
				splMsgs.push_back("");
			}
		}
	}

	if(tmpStr.size() != 0)
	{
		splMsgs.push_back(tmpStr);
	}
}

	
void GPSMsgParser::coor_format_convert(double &lat, char ns, double &lon, char ew)
{
	int degree;
	double min; 

	// Convert latitue from ddmm.mmmm format to singed degrees format	
	degree	= lat / 100;
	min	= lat - degree * 100;
	lat	= degree + min / 60;
	if(ns == 'S')
		lat *= -1;	

	// Convert longitute from dddmm.mmmm format to singed degrees format	
	degree	= lon / 100;
	min	= lon - degree * 100;
	lon	= degree + min / 60;
	if(ns == 'W')
		lon *= -1;	
}

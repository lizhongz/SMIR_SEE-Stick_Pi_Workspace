#ifndef _GPS_MSG_PARSER_
#define _GPS_MSG_PARSER_

#include <string>
#include <vector>

using namespace std;

// RMC data 
typedef struct
{
	string	utc;		// UTC time
	char	status;		// Data status(A = valid, V= not valid
	double	lat;		// Latitude of fix
	double	lon;		// Longitude od fix	
	double	vel;		// Speed over the ground in knots
	double	azim;		// True course in degrees
	double	mag_var;		// Magnetic variation in degrees
}RMC_data;

// GGA data
typedef struct
{
	string	utc;		// UTC time
	double	lat;		// Latitude of fix
	double	lon;		// Longitude od fix	
	double	alt;		// Antenna altitude in meters
	int	sat_num;	// Number of satellites in Use
}GGA_data;

class GPSMsgParser
{
public:
	
	// Parse RMC message
	static void parse_rmc_msg(const string &rawMsg, RMC_data &rmcData);

	// Parse GGA message
	static void parse_gga_msg(const string &rawMsg, GGA_data &ggaData);
	
private:

	// Split raw messege into string array by delim
	static void split(const string &rawMsg, const char delim, vector<string> &splMsgs);

	
	// Convert coordinate format from ddmm.mmmm format to singed degrees format	
	static void coor_format_convert(double &lat, char ns, double &lon, char ew);
	
};

#endif //_GPS_MSG_PARSER_

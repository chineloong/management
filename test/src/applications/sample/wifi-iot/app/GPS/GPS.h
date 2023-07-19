#ifndef __GPS_H
#define __GPS_H

#include "string.h"

typedef struct {
	char time[10];
	double latitude;
	double longitude;
	int fix_quality;
	int num_satellites;
	double hdop;
	double altitude;
	float speed;
	float direction;
} GPS_Data;


extern GPS_Data data;

GPS_Data parse_GPGGA(char *gps_string);
GPS_Data parse_GPRMC(char *gps_string);
GPS_Data parse_GPGLL(char *gps_string);


#endif

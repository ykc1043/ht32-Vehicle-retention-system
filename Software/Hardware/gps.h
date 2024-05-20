#ifndef __GPS_H__
#define __GPS_H__

#include "usart.h"


void errorLog(int num);
void parseGpsBuffer(void);
void printGpsBuffer(void);
float convertToDecimalDegrees(const char* ddmm, int isLatitude);
void printConvertedCoordinates(const char* latitude, const char* N_S, const char* longitude, const char* E_W);
void GPS_GetData(float *data);


#endif

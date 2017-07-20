#include <stdio.h>
#include <stdlib.h>
#include "GPSparsed.h"

extern GPSparsedTotal* GPSdata;

int main() {
	
	FILE * fp = fopen("GPS_RAW_DATA.txt", "rt");
	if (fp == NULL) exit(1);

	GPS_parseDataFromFile(fp);
	GPS_makeFile();
}
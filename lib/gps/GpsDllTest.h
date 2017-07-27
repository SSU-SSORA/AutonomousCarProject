#ifdef GPSDLLTEST_EXPORTS
#define GPSDLLTEST_API __declspec(dllexport)
#else
#define GPSDLLTEST_API __declspec(dllimport) 
#endif

#ifndef GPRMC_LENGTH
#define GPRMC_LENGTH 6
#endif

#include <stdio.h>


typedef struct {
	double		UTCtime;					//	Field2
	char		Status;						//	Field3
	double		Latitude;					//	Field4
	char		N_S_Indicator;				//	Field5
	double		Longtitude;					//	Field6
	char		E_W_Indicator;				//	Field7
	double		Speed_Over_Ground;			//	Field8

	double		Course_Over_Ground;			//	Field9
	char*		UTCdate;					//	Field10
	double		Magnetic_Variation;			//	Field11
	char*		CheckSum;					//	Field12
}GPSparsed;

typedef struct {
	GPSparsed *head;//GPRMC 데이터 배열을 가리킬 포인터
	size_t length;//저장한 데이터의 갯수
}GPSparsedTotal;

GPSDLLTEST_API int GPS_parseDataFromFile(FILE * targetFile);//파일로부터 정보를 읽어오는 함수
GPSDLLTEST_API void GPS_makeFile();//GPSparsed.txt생성
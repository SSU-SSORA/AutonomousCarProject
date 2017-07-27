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
	GPSparsed *head;//GPRMC ������ �迭�� ����ų ������
	size_t length;//������ �������� ����
}GPSparsedTotal;

GPSDLLTEST_API int GPS_parseDataFromFile(FILE * targetFile);//���Ϸκ��� ������ �о���� �Լ�
GPSDLLTEST_API void GPS_makeFile();//GPSparsed.txt����
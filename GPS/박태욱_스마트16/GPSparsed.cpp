#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "GPSparsed.h"

GPSparsedTotal * GPSdata;


static void DEBUG_showGPSdata(const GPSparsed *target) {
	printf("UTCtime             : %lf	\n", target->UTCtime			);
	printf("Status              : %c	\n", target->Status				);
	printf("Latitude            : %lf	\n", target->Latitude			);
	printf("N/S Indicator       : %c	\n", target->N_S_Indicator		);
	printf("Longtitude          : %lf	\n", target->Longtitude			);
	printf("E/W Indicator       : %c	\n", target->E_W_Indicator		);
	printf("Speed Over Ground   : %lf	\n", target->Speed_Over_Ground	);
	printf("Course Over Ground  : %lf	\n", target->Course_Over_Ground	);
	printf("UTC Date            : %s	\n", target->UTCdate			);
	printf("Magnetic variation  : %lf	\n", target->Magnetic_Variation	);
	printf("Checksum            : %s	\n", target->CheckSum			);
}

static void clearData()
/*
GPSdata의 메모리를 해제하는 함수이다.
GPSdata변수의 NULL체크 기능도 포함됨
*/
{
	if (GPSdata == NULL)
		return;
	for (int i = 0; i < GPSdata->length; ++i) {
		free(GPSdata->head[i].CheckSum);
		free(GPSdata->head[i].UTCdate);
	}
	free(GPSdata->head);
	free(GPSdata);
	GPSdata = NULL;
}

static void readGPRMC(char * targetLine) 
/*
이 함수는 GPRMC로 시작하는 라인이 유효함을 가정하고 동작함
#C++코드로 바꾸면서 throw 구문을 추가할 것

Step 1. GPSparse 메모리 할당
Step 2. 각 Field parsing 후 저장
Step 3. GPSparseTotal의 head 배열 메모리 추가 후 갱신, 새로 생긴 정보도 기록
*/
{
	//Step 1. 
	//GPSparsed 구조체 생성
	GPSparsed  temp = { 0 };

	//Step 2. 
	//read Field 2~11
	const char * ignore = ", ";
	char * commaPointer = strtok(targetLine, ignore);
	//포인터가 $를 가리킴

	//Field2
	commaPointer = strtok(NULL, ignore);//첫 번째 콤마 위치를 가리킴
	temp.UTCtime = atof(commaPointer);

	//Field3
	commaPointer = strtok(NULL, ignore);
	temp.Status = *commaPointer;

	//Field4
	commaPointer = strtok(NULL, ignore);
	temp.Latitude = atof(commaPointer);

	//Field5
	commaPointer = strtok(NULL, ignore);
	temp.N_S_Indicator = *commaPointer;

	//Field6
	commaPointer = strtok(NULL, ignore);
	temp.Longtitude = atof(commaPointer);

	//Field7
	commaPointer = strtok(NULL, ignore);
	temp.E_W_Indicator = *commaPointer;

	//Field8
	commaPointer = strtok(NULL, ignore);
	temp.Speed_Over_Ground = atof(commaPointer);

	//Field9
	commaPointer = strtok(NULL, ignore);
	temp.Course_Over_Ground = atof(commaPointer);

	//Field10
	commaPointer = strtok(NULL, ignore);
	temp.UTCdate = (char *)malloc(sizeof(char) * strlen(commaPointer) + 1);
	strcpy(temp.UTCdate, commaPointer);

	//Field11-->매우 자주 누락됨
	
	commaPointer = strtok(NULL, ignore);
	if (!isalpha(*commaPointer)) {//Field11이 존재하는 경우
		temp.Magnetic_Variation = atof(commaPointer);
		
		commaPointer = strtok(NULL, ignore);
	}
	else {
		temp.Magnetic_Variation = 0;
	}

	//Field12
	temp.CheckSum = (char *)malloc(sizeof(char) * strlen(commaPointer) + 1);
	strcpy(temp.CheckSum, commaPointer);


	


	//Step3. head 포인터 길이연장 / 새로운 데이터 저장
	GPSparsed * tempHead = (GPSparsed *)malloc(sizeof(GPSparsed) * (GPSdata->length + 1));//길이 연장
	if (GPSdata->head != NULL) {//기존 head포인터에 정보가 존재한다면
		memcpy(tempHead, GPSdata->head, sizeof(GPSparsed) * GPSdata->length);//정보 복사
		free(GPSdata->head);//head포인터 데이터 해제
	}
	tempHead[GPSdata->length] = temp;//마지막 인덱스에 접근하여 데이터 저장
	GPSdata->head = tempHead;
	GPSdata->length++;

	//Step4. DEBUG
	DEBUG_showGPSdata(&temp);
	printf("\n\n");
}

int GPS_parseDataFromFile (FILE * targetFile)
/*
파라미터의 파일포인터를 이용해 GPS데이터를 파싱하고 
모든 데이터를 GPSparsedTotal 자료형에 저장하여 반환한다.

만약 데이터파싱을 전혀 수행하지 못한 경우 -1을 반환한다.
*/
{
	//파일 커서 초기화
	fseek(targetFile, SEEK_SET, 0);

	//Total구조체 초기화
	clearData();
	GPSdata = (GPSparsedTotal *)calloc(1, sizeof(GPSparsedTotal));
	

	//파일 파싱 시작
	char GPRMCcheck[200] = { 0 };
	const int checkLength = sizeof(GPRMCcheck) 
		/sizeof(GPRMCcheck[0]);
	while (!feof(targetFile)) {

		//한줄 파싱
		fgets(GPRMCcheck, checkLength, targetFile);

		//유효성 검사
		if (GPRMCcheck[0] != '$') continue;//맨 마지막 라인 체크용
		if (strncmp(GPRMCcheck, "$GPRMC",GPRMC_LENGTH ) == 0) {//GPRMC인 경우
			readGPRMC(GPRMCcheck);
		}
		
	}
	if(GPSdata->head != NULL)
		return 1;
	else return -1;
}

void GPS_makeFile() {
	FILE * fp = fopen("GPSparsed.txt", "w");
	for (int i = 0; i < GPSdata->length; ++i) {
		fprintf(fp,"UTCtime             : %lf	\n", GPSdata->head[i].UTCtime);
		fprintf(fp,"Status              : %c	\n", GPSdata->head[i].Status);
		fprintf(fp,"Latitude            : %lf	\n", GPSdata->head[i].Latitude);
		fprintf(fp,"N/S Indicator       : %c	\n", GPSdata->head[i].N_S_Indicator);
		fprintf(fp,"Longtitude          : %lf	\n", GPSdata->head[i].Longtitude);
		fprintf(fp,"E/W Indicator       : %c	\n", GPSdata->head[i].E_W_Indicator);
		fprintf(fp,"Speed Over Ground   : %lf	\n", GPSdata->head[i].Speed_Over_Ground);
		fprintf(fp,"Course Over Ground  : %lf	\n", GPSdata->head[i].Course_Over_Ground);
		fprintf(fp,"UTC Date            : %s	\n", GPSdata->head[i].UTCdate);
		fprintf(fp,"Magnetic variation  : %lf	\n", GPSdata->head[i].Magnetic_Variation);
		fprintf(fp,"Checksum            : %s	\n", GPSdata->head[i].CheckSum);
		fprintf(fp, "\n");
	}
	fclose(fp);
}
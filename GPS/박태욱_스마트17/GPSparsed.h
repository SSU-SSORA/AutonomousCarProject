#pragma once
//C언어의 예외처리
// /0, (구조체포인터)->alpha;
// 파일 오픈 시 파일포인터가 NULL일때 등
//1. 함수의 반환값
//2. errno 전역변수
//프로그래머가 의도하지 않은 에러가 발생한다->프로그램 kill
//C++ : 컴파일타임에 검사할수 있는 다양한 도구를 제공
//java : exception 문법을 통해서 에러를 핸들링

#ifndef GPRMC_LENGTH
#define GPRMC_LENGTH 6
#endif



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

//1. GPS_DATA_RAW에서 $GPRMC를 발견한 경우
//1-1. temp = (GPSparsed *)malloc(sizeof(GPSparsed) * (기존자료갯수 + 1));
//형태로 메모리를 할당한다.
//1-2. memcpy함수를 이용해 head포인터에 이미 저장되어있던 모든 내용을 temp에 복사한다.
//그 후 free(head)
//1-3. temp[제일 마지막 인덱스] = (새로 파싱한 데이터) 를 수행하여 새로운 정보를 temp
//배열에 저장한다
//1-4. head = temp 명령을 통해 temp의 내용을 head로 복사한다.

/*
위 작업을 성공적으로 해내면 head포인터를 이용해 모든 GPRMC데이터에 접근이 가능해 짐
이 헤드포인터를 이용해 정보를 파일에 저장하는 함수를 작성한다!
*/



int GPS_parseDataFromFile(FILE * targetFile);//파일로부터 정보를 읽어오는 함수
void GPS_makeFile();//GPSparsed.txt생성
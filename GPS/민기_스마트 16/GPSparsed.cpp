#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "GPSparsed.h"
using namespace std;

GPSparsedTotal total;

string getGPS(string file_name) {

	string line;
	ifstream inf(file_name);
	GPSparsed* temp;
	size_t index = 0;
	char* token = NULL;
	char*  str2 = ",";//','이 들어있는 문자열을 분리
	getline(inf, line);

	if ((line.find("$GPRMC", 0)) != std::string::npos) {
		while ((index = line.find(",", index)) != std::string::npos) { 
			line.replace(index, line.length(), " "); 
			index += line.length(); 
		}
		stringstream line2(line);
		temp = (GPSparsed *)malloc(sizeof(GPSparsed)* (total.length+1));
		memcpy(temp, total.head, total.length);
		free(total.head);

		line2 >> token;
		temp[total.length].UTCtime = (double)*token;
		cout << temp[total.length].UTCtime;

		line2 >> token;
		temp[total.length].Status = (char)*token;
		cout << temp[total.length].Status;

		line2 >> token;
		temp[total.length].Latitude = (double)*token;

		line2 >> token;
		temp[total.length].N_S_Indicator = (char)*token;

		line2 >> token;
		temp[total.length].Longtitude = (double)*token;

		line2 >> token;
		temp[total.length].E_W_Indicator = (char)*token;

		line2 >> token;
		temp[total.length].Speed_Over_Ground = (double)*token;

		line2 >> token;
		temp[total.length].Course_Over_Ground = (double)*token;
		
		line2 >> token;
		temp[total.length].UTCdate = (char *)malloc(sizeof(token));
		strcpy(temp[total.length].UTCdate,token);

		line2 >> token;
		temp[total.length].Magnetic_Variation = (double)*token;

		line2 >> token;
		temp[total.length].CheckSum = (char *)malloc(sizeof(token));
		strcpy(temp[total.length].CheckSum,token);

		total.head = temp;
		total.length++;

		inf.close();
		return line;
	}
	ofstream of(file_name);
	
	getGPS(file_name);
}


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
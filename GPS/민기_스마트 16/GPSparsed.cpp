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
	char*  str2 = ",";//','�� ����ִ� ���ڿ��� �и�
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


//1. GPS_DATA_RAW���� $GPRMC�� �߰��� ���
//1-1. temp = (GPSparsed *)malloc(sizeof(GPSparsed) * (�����ڷ᰹�� + 1));
//���·� �޸𸮸� �Ҵ��Ѵ�.
//1-2. memcpy�Լ��� �̿��� head�����Ϳ� �̹� ����Ǿ��ִ� ��� ������ temp�� �����Ѵ�.
//�� �� free(head)
//1-3. temp[���� ������ �ε���] = (���� �Ľ��� ������) �� �����Ͽ� ���ο� ������ temp
//�迭�� �����Ѵ�
//1-4. head = temp ����� ���� temp�� ������ head�� �����Ѵ�.

/*
�� �۾��� ���������� �س��� head�����͸� �̿��� ��� GPRMC�����Ϳ� ������ ������ ��
�� ��������͸� �̿��� ������ ���Ͽ� �����ϴ� �Լ��� �ۼ��Ѵ�!
*/
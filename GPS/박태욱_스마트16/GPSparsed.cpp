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
GPSdata�� �޸𸮸� �����ϴ� �Լ��̴�.
GPSdata������ NULLüũ ��ɵ� ���Ե�
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
�� �Լ��� GPRMC�� �����ϴ� ������ ��ȿ���� �����ϰ� ������
#C++�ڵ�� �ٲٸ鼭 throw ������ �߰��� ��

Step 1. GPSparse �޸� �Ҵ�
Step 2. �� Field parsing �� ����
Step 3. GPSparseTotal�� head �迭 �޸� �߰� �� ����, ���� ���� ������ ���
*/
{
	//Step 1. 
	//GPSparsed ����ü ����
	GPSparsed  temp = { 0 };

	//Step 2. 
	//read Field 2~11
	const char * ignore = ", ";
	char * commaPointer = strtok(targetLine, ignore);
	//�����Ͱ� $�� ����Ŵ

	//Field2
	commaPointer = strtok(NULL, ignore);//ù ��° �޸� ��ġ�� ����Ŵ
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

	//Field11-->�ſ� ���� ������
	
	commaPointer = strtok(NULL, ignore);
	if (!isalpha(*commaPointer)) {//Field11�� �����ϴ� ���
		temp.Magnetic_Variation = atof(commaPointer);
		
		commaPointer = strtok(NULL, ignore);
	}
	else {
		temp.Magnetic_Variation = 0;
	}

	//Field12
	temp.CheckSum = (char *)malloc(sizeof(char) * strlen(commaPointer) + 1);
	strcpy(temp.CheckSum, commaPointer);


	


	//Step3. head ������ ���̿��� / ���ο� ������ ����
	GPSparsed * tempHead = (GPSparsed *)malloc(sizeof(GPSparsed) * (GPSdata->length + 1));//���� ����
	if (GPSdata->head != NULL) {//���� head�����Ϳ� ������ �����Ѵٸ�
		memcpy(tempHead, GPSdata->head, sizeof(GPSparsed) * GPSdata->length);//���� ����
		free(GPSdata->head);//head������ ������ ����
	}
	tempHead[GPSdata->length] = temp;//������ �ε����� �����Ͽ� ������ ����
	GPSdata->head = tempHead;
	GPSdata->length++;

	//Step4. DEBUG
	DEBUG_showGPSdata(&temp);
	printf("\n\n");
}

int GPS_parseDataFromFile (FILE * targetFile)
/*
�Ķ������ ���������͸� �̿��� GPS�����͸� �Ľ��ϰ� 
��� �����͸� GPSparsedTotal �ڷ����� �����Ͽ� ��ȯ�Ѵ�.

���� �������Ľ��� ���� �������� ���� ��� -1�� ��ȯ�Ѵ�.
*/
{
	//���� Ŀ�� �ʱ�ȭ
	fseek(targetFile, SEEK_SET, 0);

	//Total����ü �ʱ�ȭ
	clearData();
	GPSdata = (GPSparsedTotal *)calloc(1, sizeof(GPSparsedTotal));
	

	//���� �Ľ� ����
	char GPRMCcheck[200] = { 0 };
	const int checkLength = sizeof(GPRMCcheck) 
		/sizeof(GPRMCcheck[0]);
	while (!feof(targetFile)) {

		//���� �Ľ�
		fgets(GPRMCcheck, checkLength, targetFile);

		//��ȿ�� �˻�
		if (GPRMCcheck[0] != '$') continue;//�� ������ ���� üũ��
		if (strncmp(GPRMCcheck, "$GPRMC",GPRMC_LENGTH ) == 0) {//GPRMC�� ���
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
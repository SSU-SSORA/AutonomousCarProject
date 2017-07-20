#include<stdio.h>
#include"GPSparsed.h"

int main();

void GPSRMCparsing();

void GPSparsing(GPSparsedTotal* GT);

void headfree(GPSparsedTotal* GT);

int main() {

	//GPSparsedTotal *GT;

	GPSRMCparsing();

	/*FILE* GPRMC = NULL;
	char SenId[7];
	double		UTCtime;					//	Field2
	char		Status;						//	Field3
	double		Latitude;					//	Field4
	char		N_S_Indicator;				//	Field5
	double		Longtitude;					//	Field6
	char		E_W_Indicator;				//	Field7
	double		Speed_Over_Ground;			//	Field8
	double		Course_Over_Ground;			//	Field9
	char		UTCdate[7];				//	Field10
	double		Magnetic_Variation;			//	Field11
	char		CheckSum[5];				//	Field12

	GPRMC = fopen("GPSRMC_PARSED.txt", "r");
	if (GPRMC == NULL)
	{
		printf("theres no data\n");
		exit(0);
	}
	while (1) {
		if (fscanf(GPRMC, "%s %lf %c %lf %c %lf %c %lf %lf %s %s", SenId, &UTCtime, &Status, &Latitude, &N_S_Indicator, &Longtitude, &E_W_Indicator, &Speed_Over_Ground
		, &Course_Over_Ground, UTCdate, CheckSum) == -1) { break; }
		if(!strcmp(SenId,"$GPRMC"))
				printf("%s %lf %c %lf %c %lf %c %lf %lf %s %s \n", SenId, UTCtime, Status, Latitude, N_S_Indicator, Longtitude, E_W_Indicator, Speed_Over_Ground
					, Course_Over_Ground, UTCdate, CheckSum);
		}

	fclose(GPRMC);*/


	/*GT = (GPSparsedTotal*)calloc(0, sizeof(GPSparsedTotal) * 1);
	GT->head = (GPSparsed *)calloc(0,sizeof(GPSparsed) * 1);
	GT->head->UTCtime = 0;
	GT->head->Status = NULL;
	GT->head->Latitude = 0;
	GT->head->N_S_Indicator = NULL;
	GT->head->Longtitude = 0;
	GT->head->E_W_Indicator = NULL;
	GT->head->Speed_Over_Ground = 0;
	GT->head->Course_Over_Ground = 0;
	GT->head->UTCdate = NULL;
	GT->head->Magnetic_Variation = 0;
	GT->head->CheckSum = 0;

	
	GPSparsing(GT);*/

	return 0;

}

void GPSRMCparsing() {
	FILE* GpsRaw = NULL;
	FILE* GPSRMC = NULL;

	char content[100];
	char* tokcontent;
	GpsRaw = fopen("GPS_RAW_DATA.txt", "r");
	if (GpsRaw == NULL)
	{
		printf("theres no data\n");
		exit(0);
	}
	GPSRMC = fopen("GPSRMC_PARSED.txt", "w");
	while (1) {
		if (fscanf(GpsRaw, "%s", content) == -1) { break; }

		tokcontent = strtok(content, ",");

		if (!strcmp(content, "$GPRMC")) {
			while (tokcontent != NULL) {
				fprintf(GPSRMC, "%s", tokcontent);
				tokcontent = strtok(NULL, ",");
				fprintf(GPSRMC, " ");
			}
		}
		fprintf(GPSRMC, "\n");
	}

	fclose(GpsRaw);
}

void GPSparsing(GPSparsedTotal* GT) {
	FILE* GPRMC = NULL;
	char SenId[7];
	GPSparsed* temp = NULL;
	GT->length = 1;
	GPRMC = fopen("GPSRMC_PARSED.txt", "r");
	if (GPRMC == NULL)
	{
		printf("theres no data\n");
		exit(0);
	}
	while (1) {
		
		temp = (GPSparsed *)malloc(sizeof(GPSparsed) * (GT->length));
		for (int i = 0; i < GT->length; i++) {
			temp[i].UTCdate = (char*)malloc(sizeof(char) * 20);
			temp[i].CheckSum = (char*)malloc(sizeof(char) * 20);
		}
		memcpy(temp, GT->head);
		headfree(GT);
		
		if (fscanf(GPRMC, "%s %lf %c %lf %c %lf %c %lf %lf %c %c", SenId, &temp[GT->length-1].UTCtime, &temp[GT->length-1].Status, &temp[GT->length-1].Latitude, &temp[GT->length-1].N_S_Indicator, 
			&temp[GT->length-1].Longtitude, &temp[GT->length-1].E_W_Indicator, &temp[GT->length-1].Speed_Over_Ground, &temp[GT->length-1].Course_Over_Ground, temp[GT->length-1].UTCdate, 
			 temp[GT->length-1].CheckSum) == -1) {
			GT->head = (GPSparsed *)malloc(sizeof(GPSparsed) * (GT->length));
			for (int i = 0; i < GT->length; i++) {
				GT->head[i].UTCdate = (char*)malloc(sizeof(char) * 20);
				GT->head[i].CheckSum = (char*)malloc(sizeof(char) * 20);
			}
			memcpy(GT->head, temp);
			for (int i = 0; i < GT->length; i++) {
				free(temp[i].UTCdate);
				free(temp[i].CheckSum);
			}
			free(temp); 
			break; }
		GT->head = (GPSparsed *)malloc(sizeof(GPSparsed) * (GT->length));
		for (int i = 0; i < GT->length; i++) {
			GT->head[i].UTCdate = (char*)malloc(sizeof(char) * 20);
			GT->head[i].CheckSum = (char*)malloc(sizeof(char) * 20);
		}
		memcpy(GT->head, temp);
		for (int i = 0; i < GT->length; i++) {
			free(temp[i].UTCdate);
			free(temp[i].CheckSum);
		}
		free(temp);
		GT->length = GT->length + 1;
		
	}
	printf("%d", GT->head[3].UTCtime);
}

void headfree(GPSparsedTotal* GT) {
	if (GT->head == NULL) {
		//일부로 비움
	}
	else {
		for (int i = 0; i < GT->length; i++) {
			free(GT->head[i].UTCdate);
			free(GT->head[i].CheckSum);
		}
		free(GT->head);
	}
}
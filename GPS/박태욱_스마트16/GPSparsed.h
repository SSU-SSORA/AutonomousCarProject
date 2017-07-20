#pragma once
//C����� ����ó��
// /0, (����ü������)->alpha;
// ���� ���� �� ���������Ͱ� NULL�϶� ��
//1. �Լ��� ��ȯ��
//2. errno ��������
//���α׷��Ӱ� �ǵ����� ���� ������ �߻��Ѵ�->���α׷� kill
//C++ : ������Ÿ�ӿ� �˻��Ҽ� �ִ� �پ��� ������ ����
//java : exception ������ ���ؼ� ������ �ڵ鸵

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
	GPSparsed *head;//GPRMC ������ �迭�� ����ų ������
	size_t length;//������ �������� ����
}GPSparsedTotal;

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



int GPS_parseDataFromFile(FILE * targetFile);//���Ϸκ��� ������ �о���� �Լ�
void GPS_makeFile();//GPSparsed.txt����
#pragma once
#include "data.h"
#include "keys.h"


class GPSConcreteData :DataComposite{
	//Clone���θ� GPSConcreteData�� �ν��Ͻ��� ������ �� ����

	//���� ������ ����
	GPSConcreteData& operator=(const GPSConcreteData &) = delete;
public:
	void SetKeys(Keys keys_) {
		if (GPSConcreteData::instance) {
			this->keys = keys;
			makeInstance();
		}
		else {
			std::cout << "GPSConcreteData�� SetKey�� �ѹ��� �۵��˴ϴ�!" << std::endl;
			exit(1);
		}
	}

	DataComposite * Clone() {
		return makeInstance();
	}

	


	~GPSConcreteData() {
		for (auto ptr : this->concreteDataType)
			delete ptr;
	}

private:
	GPSConcreteData() {}
	GPSConcreteData * makeInstance() {
		for (auto& pair : keys.GetKeyAry) {
			switch (pair.second) {
			case DataType::CHAR:
				this->concreteDataType.push_back(make_data((char)0));
			case DataType::DOUBLE:
				this->concreteDataType.push_back(make_data((double)0));
			case DataType::STRING:
				this->concreteDataType.push_back(make_data(""));
			case DataType::INT:
				this->concreteDataType.push_back(make_data((int)0));
			}
		}
	}

private:
	static Keys keys;
	static GPSConcreteData * instance;
};

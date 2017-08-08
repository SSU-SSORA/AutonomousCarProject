#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "DataType.h"

struct Keys {
	typedef std::vector<std::pair<std::string, DataType>> keyType;
	void AddKey(std::string keyName, DataType type) {
		keyAry.push_back(std::make_pair(keyName, type));
	}
	keyType GetKeyAry() const {
		return keyAry;
	}
private:
	std::vector < std::pair<std::string, DataType>> keyAry;
	
};
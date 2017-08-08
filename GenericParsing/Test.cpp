#include <iostream>
#include <vector>
#include "data.h"
#include "DataType.h"

int main() {
	Data* a = make_data(3.14);
	std::vector<Data *> vec;
	vec.push_back(a);
	delete vec[0];
	vec.clear();
}
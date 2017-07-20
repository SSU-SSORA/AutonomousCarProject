#include <iostream>
#include <string>
#include "GPSparsed.h"

using namespace std;

int main() {
	string filename = "GPS_RAW_DATA.txt";
	string a = "";
	a = getGPS(filename);
	cout << a << endl;
	return 0;
}
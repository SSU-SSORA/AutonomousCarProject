
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DataType.h"



class Data {
public:
	virtual std::ostream& operator<<(std::ostream& os) = 0;
	virtual std::istream& operator>>(std::istream& os) = 0;
	virtual std::ofstream& operator<<(std::ofstream& os) = 0;
	virtual std::ifstream& operator>>(std::ifstream& os) = 0;

	friend std::ostream& operator<<(std::ostream& os, Data& data) {
		return data << os;
	}
	friend std::istream& operator>>(std::istream& is, Data& data) {
		return data >> is;
	}
	friend std::ofstream& operator<<(std::ofstream& ofs, Data& data) {
		return data << ofs;
	}
	friend std::ifstream& operator>>(std::ifstream& ifs, Data& data) {
		return data >> ifs;
	}
	virtual ~Data() { std::cout << "destructor\n"; }
	virtual DataType GetType() const = 0;
};

class Data_int : public Data{
	
public:
	typedef int _Ty;//이 문장만 수정하면 됨
	Data_int(_Ty d) : d(d) {}

	std::ostream& operator<<(std::ostream& os) {
		os << d;
		return os;
	}
	std::istream& operator>>(std::istream& is) {
		is >> d;
		return is;
	}
	std::ofstream& operator<<(std::ofstream& ofs) {
		ofs << d;
		return ofs;
	}
	std::ifstream& operator>>(std::ifstream& ifs) {
		ifs >> d;
		return ifs;
	}
	DataType GetType() const {
		return DataType::INT;
	}
private:
	_Ty d;
};

class Data_double: public Data{
	typedef double _Ty;//이 문장만 수정하면 됨
public:
	Data_double(_Ty d) : d(d) {}

	std::ostream& operator<<(std::ostream& os) {
		os << d;
		return os;
	}
	std::istream& operator>>(std::istream& is) {
		is >> d;
		return is;
	}
	std::ofstream& operator<<(std::ofstream& ofs) {
		ofs << d;
		return ofs;
	}
	std::ifstream& operator>>(std::ifstream& ifs) {
		ifs >> d;
		return ifs;
	}
	DataType GetType() const {
		return DataType::DOUBLE;
	}
private:
	_Ty d;
};

class Data_char : public Data {
	typedef char _Ty;//이 문장만 수정하면 됨
public:
	Data_char(_Ty d) : d(d) {}

	std::ostream& operator<<(std::ostream& os) {
		os << d;
		return os;
	}
	std::istream& operator>>(std::istream& is) {
		is >> d;
		return is;
	}
	std::ofstream& operator<<(std::ofstream& ofs) {
		ofs << d;
		return ofs;
	}
	std::ifstream& operator>>(std::ifstream& ifs) {
		ifs >> d;
		return ifs;
	}
	DataType GetType() const {
		return DataType::CHAR;
	}
private:
	_Ty d;
};

class Data_string : public Data{
	typedef std::string _Ty;
public:
	Data_string(_Ty d) :d(d){}

	std::ostream& operator<<(std::ostream& os) {
		os << d;
		return os;
	}
	std::istream& operator>>(std::istream& is) {
		is >> d;
		return is;
	}
	std::ofstream& operator<<(std::ofstream& ofs) {
		ofs << d;
		return ofs;
	}
	std::ifstream& operator>>(std::ifstream& ifs) {
		ifs >> d;
		return ifs;
	}
	DataType GetType() const {
		return DataType::STRING;
	}
private:
	_Ty d;
};

class Keys;

//Composite Pattern
class DataComposite : Data {
	typedef Data super;
public:
	virtual ~DataComposite() {}

	std::vector<Data *>	getDataAry() const {
		return this->concreteDataType;
	}
protected:
	Data* make_data(int v) {
		return new Data_int(v);
	}
	Data* make_data(char v) {
		return new Data_char(v);
	}
	Data* make_data(double v) {
		return new Data_double(v);
	}
	Data* make_data(std::string v) {
		return new Data_string(v);
	}

	//make sure that Derived class have 'keys' member.
	virtual void SetKeys(Keys keys) = 0;//전방선언을 통해 사용
	//and make new Instance of concreteDataType;

	//To make it easy to copy Datatype, implement the Clone of self instance
	virtual DataComposite * Clone() = 0;
	

protected:
	std::vector<Data *> concreteDataType;

private:
};
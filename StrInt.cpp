// StrInt.cpp

#include "StrInt.h"

StrInt StrInt::divTwo(size_t length){
	// Base case
	unsigned short lastDigit = m_numberString[length - 1] - '0';
	if (length == 1){
		return StrInt(m_negative ? -lastDigit / 2 : lastDigit / 2);
	}

	// Recursive case
	bool odd = (m_numberString[length - 2] - '0') % 2 == 1;
	StrInt number = divTwo(length - 1);

	if (number.m_numberString[0] == '0'){
		number.m_numberString.clear();
		number.m_negative = m_negative;
	}

	if (odd){
		number.m_numberString.push_back(lastDigit / 2 + 5 + '0');
	}
	else{
		number.m_numberString.push_back(lastDigit / 2 + '0');
	}

	return number;
}

void StrInt::normalize(){
	if (m_numberString == "0"){
		m_negative = false;
	}
}

StrInt::StrInt() : StrInt(0){}
StrInt::StrInt(int number){
	m_numberString.reserve(MAX_RESERVED_LENGTH);

	if (number == 0){
		m_negative = false;
		m_numberString.push_back('0');
		return;
	}

	std::string temp;
	if (m_negative = number < 0 ? true : false){
		number = -number;
	}
	while (number){
		temp.push_back((number % 10) + '0');
		number /= 10;
	}
	for (int i = temp.length() - 1; i >= 0; i--)
		m_numberString.push_back(temp[i]);
}

std::ostream& operator<<(std::ostream& os, const StrInt& number){
	os << (number.m_negative ? "-" : "") << number.m_numberString;

	return os;
}
std::istream& operator>>(std::istream& is, StrInt& number){
	std::string buffer;
	is >> buffer;

	for (size_t i = 1; i < buffer.length(); i++){
		if (buffer[i] < '0' || buffer[i] > '9'){
			return is;
		}
	}

	if (buffer.length() < 1 || ((buffer[0] == '-' || buffer[0] == '+') && buffer.length() == 1)){
		return is;
	}

	number.m_negative = buffer[0] == '-';
	number.m_numberString.clear();
	for (size_t i = (buffer[0] == '-' || buffer[0] == '+') ? 1 : 0; i < buffer.length(); i++)
		number.m_numberString.push_back(buffer[i]);

	number.normalize();

	return is;
}

unsigned short StrInt::lastDigit(){
	return m_numberString[m_numberString.length() - 1] - '0';
}

bool StrInt::isEven(){
	return lastDigit() % 2 == 0;
}

bool StrInt::isZero(){
	return m_numberString == "0";
}

bool StrInt::isNegative(){
	return m_negative;
}

void StrInt::toNegative(){
	if (m_numberString != "0"){
		m_negative = true;
	}
}
void StrInt::toPositive(){
	m_negative = false;
}

StrInt StrInt::Half(){
	return divTwo(m_numberString.length());
}
StrInt StrInt::Double(){
	StrInt doub;
	doub.m_numberString.clear();

	std::string temp;
	unsigned short accumulator = 0;
	for (int i = m_numberString.length() - 1; i >= 0; i--){
		unsigned short digit = m_numberString[i] - '0';
		digit = digit * 2 + accumulator;
		accumulator = digit / 10;
		digit %= 10;
		temp.push_back(digit + '0');
	}

	if (accumulator > 0){
		temp.push_back(accumulator + '0');
	}

	for (int i = temp.length() - 1; i >= 0; i--){
		doub.m_numberString.push_back(temp[i]);
	}

	doub.m_negative = m_negative;
	doub.normalize();

	return doub;
}

StrInt& StrInt::operator++(){
	std::string temp;
	unsigned short accumulator = m_negative ? -1 : 1;
	for (int i = m_numberString.length() - 1; i >= 0; i--){
		char c = m_numberString[i] + accumulator;
		if (c < '0'){
			c = '9';
		}
		else if (c > '9'){
			c = '0';
		}
		else{
			accumulator = 0;
		}
		temp.push_back(c);
	}

	if (temp.back() == '0' && accumulator == 0){
		temp.pop_back();
	}

	m_numberString.clear();
	if (accumulator == 1){
		m_numberString.push_back('1');
	}
	for (int i = temp.length() - 1; i >= 0; i--){
		m_numberString.push_back(temp[i]);
	}

	normalize();

	return *this;
}
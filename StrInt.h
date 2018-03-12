// StrInt.h

#pragma once

#include <string>
#include <iostream>

class StrInt{
private:
	bool m_negative;
	std::string m_numberString;

	static const unsigned int MAX_RESERVED_LENGTH = 100;

	StrInt divTwo(size_t length);
	void normalize();
public:
	StrInt();
	StrInt(int number);

	friend std::ostream& operator<<(std::ostream& os, const StrInt& number);
	friend std::istream& operator>>(std::istream& is, StrInt& number);

	unsigned short lastDigit();

	bool isEven();
	bool isZero();
	bool isNegative();

	void toNegative();
	void toPositive();

	StrInt Half();
	StrInt Double();

	StrInt& operator++();
};

std::istream& operator>>(std::istream& is, StrInt& number);
std::ostream& operator<<(std::ostream& os, const StrInt& number);
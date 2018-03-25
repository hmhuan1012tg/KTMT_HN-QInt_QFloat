#pragma once
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

const int AFTER_DOT = 35; // 2^-112 ~ 10^-35
class StrFloat //save at base 10
{
	string m_float;
	bool m_negative;
public:
	StrFloat();
	StrFloat(string p);
	~StrFloat();
	bool isNegative(); //Kiểm tra số âm.
	void normalize(); //

	StrFloat& operator+(const StrFloat &p);
	StrFloat& operator=(const StrFloat &p);
	StrFloat& operator/(int p);
	StrFloat& operator*(int p);

	StrFloat operator/=(int p);
	StrFloat operator+=(const StrFloat& p);
	StrFloat operator++(int x);

	friend istream& operator>>(istream& is, StrFloat& p);
	friend ostream& operator<<(ostream& os, const StrFloat &p);
};

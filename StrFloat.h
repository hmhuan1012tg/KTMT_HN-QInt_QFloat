#pragma once
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

class StrFloat
{
	string m_float;
	bool m_negative;
public:
	StrFloat();
	StrFloat(string p);
	~StrFloat();
	bool isNegative();

	StrFloat& operator+(const StrFloat &p);
	StrFloat& operator=(const StrFloat &p);
	StrFloat& operator/(int p);
	StrFloat& operator*(int p);
	friend istream& operator>>(istream& is, StrFloat& p);
	friend ostream& operator<<(ostream& os, const StrFloat &p);
};

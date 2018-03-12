// QInt.h

#pragma once

#include "StrInt.h"
#include <cstdint>

class QInt{
private:
	uint32_t m_binary[4];
public:
	QInt();
	QInt(const QInt& x);
	QInt& operator=(const QInt& x);

	QInt operator-() const;

	bool GetBit(unsigned short position) const;
	void SetBit(unsigned short position, bool on);

	void ScanQInt();
	void PrintQInt();

	bool* DecToBin();
	static QInt BinToDec(bool* bits);

	static char* BinToHex(bool *bits);
	char* DecToHex();

	// TODO: + - * /
	QInt operator+(const QInt& q);
	QInt operator-(const QInt& q);
	QInt operator*(const QInt& q);
	QInt operator/(const QInt& q);
	
	// TODO: & | ^ ~ 
	QInt operator&(const QInt& q);
	QInt operator|(const QInt& q);
	QInt operator^(const QInt& q);
	QInt operator~();

	// TODO: << >>
	QInt operator<<(const int n);
	QInt operator>>(const int n);
};
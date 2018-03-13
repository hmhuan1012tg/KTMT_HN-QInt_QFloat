// QInt.h

#pragma once

#include "StrInt.h"
#include <cstdint>

struct QInt{
	QInt();
	QInt(const QInt& q);
	QInt& operator=(const QInt& q);

	static const unsigned short BIN_PER_HEX = 4;
	static const unsigned short HEX_PER_UINT = 8;

	static const unsigned short MAX_HEX = 15;
	static const uint32_t MAX_UINT = 0xffffffff;

	static const unsigned short BINARY = 2;
	static const unsigned short HEX = 16;
	
	static const unsigned short UINT_NUM = 4;
	
	static const unsigned short SIZE_OF_QINT = 128;

	uint32_t m_binary[UINT_NUM];
};

bool GetBit(QInt q, unsigned short position);
void SetBit(QInt& q, unsigned short position, bool on);

void ScanQInt(QInt& q);
void PrintQInt(QInt q);

std::string DecToBin(QInt q);
QInt BinToDec(const std::string& bits);

std::string BinToHex(const std::string& bits);
std::string DecToHex(QInt q);
QInt HexToDec(const std::string& hexs);

// TODO: + - * /
QInt operator+(QInt q, QInt m);
QInt operator-(QInt q, QInt m);
QInt operator*(QInt q, QInt m);
QInt operator/(QInt q, QInt m);

// TODO: & | ^ ~ 
QInt operator&(QInt q, QInt m);
QInt operator|(QInt q, QInt m);
QInt operator^(QInt q, QInt m);
QInt operator~(QInt q);

// TODO: << >>
QInt operator<<(QInt q, const int n);
QInt operator>>(QInt q, const int n);
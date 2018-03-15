//QFloat.h

#pragma once
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

class QFloat
{
	int m_el[4]; //el: element; 
public:
	QFloat();
	~QFloat();

	bool GetBit(unsigned short position);
	void SetBit(unsigned short position, bool on);
	bool CheckBitScope(int l, int r, int sd);

	void ScanQFloat();
	void PrintQFloat();

	QFloat BinToDec(bool *bit); // el[0]: 0-31; el[1]:32-63; el[2]:64-95; el[3]:96-127
	bool * DecToBin(); //  1-128: el[0] -> el[3]
};

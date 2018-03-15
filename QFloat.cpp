//QFloat.cpp

#include "QFloat.h"


bool QFloat::GetBit(unsigned short position) {
	unsigned short block = position / 32;
	unsigned short i = position % 32;
	return (m_el[block] & (1 << i)) != 0;
}
void QFloat::SetBit(unsigned short position, bool on) {
	unsigned short block = position / 32;
	unsigned short i = position % 32;
	if (on) {
		m_el[block] |= (1 << i);
	}
	else {
		m_el[block] &= (~(1 << i));
	}
}

void QFloat::ScanQFloat()
{
}

bool QFloat::CheckBitScope(int l, int r,int sd) // l=begining of bit'scope, r=end of bit's scope, sd(means standard)=value of each bit needs checking (only 0 or 1)
{
	for (int i = l; i <= r; i++)
		if (GetBit(i) != sd) return false;
	return true;
}
void QFloat::PrintQFloat()
{
	int exp=2; //exp=1 means that every bit is 1; 
			//exp=0 means that every bit is 0;
			//exp=2 means that not (every bit is 1 or 0)
	if (CheckBitScope(1, 15, 1)) exp = 1;
	if (CheckBitScope(1, 15, 0)) exp = 0;

	double value; //contains the value of bit after dot.  After finishing StrFloat, (*) convert into StrFloat 
	
	if (exp == 1) { //Check special situation of floating-point
		if (CheckBitScope(16, 127, 0)) cout << "Infinite";
		else cout << "NaN";
	}
	else
		if (exp == 0 && CheckBitScope(16, 127, 0)) cout << "0";
		else
		{
			int sign = CheckBitScope(0, 0, 1); //0: (+); 1:(-)
			int base2 = 1; //temporary variable constain 2^(x)
			double base2after = 1; //temporary variable constain 2^(-x) (*)
			int Vexp = 0; //Value of exp
			int i;
			
			for (i = 15; i > 0; i--) {
				if (GetBit(i)) Vexp += base2;
				base2 *= 2;
			}
			Vexp -= (base2 / 2 - 1);

			for (i = 16; i < 128; i++){
				base2after /= 2; //(*)
				if (GetBit(i)) value += base2after; //(*)
			}
			if (exp != 0) value++; //(*)

			if (sign) cout << '-';
			cout << value << "x2^" << Vexp;
		}
}

QFloat QFloat::BinToDec(bool * bit)
{
	QFloat p;
	for (int i = 0; i < 128; i++)
		p.SetBit(i, bit[i]);
	return p;
}

bool * QFloat::DecToBin()
{
	bool *p;
	p = new bool[128];
	for (int i = 0; i < 128; i++)
		p[i] = this->GetBit(i);
	return p;
}

QFloat::QFloat()
{
	for (int i = 0; i < 3; i++)
		m_el[i] = 0;
}

QFloat::~QFloat()
{
}

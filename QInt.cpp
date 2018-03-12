// QInt.cpp

#include "QInt.h"
#include "StrInt.h"

QInt::QInt(){
	for (int i = 0; i < 4; i++){
		m_binary[i] = 0;
	}
}
QInt::QInt(const QInt& x){
	for (int i = 0; i < 4; i++)
		m_binary[i] = x.m_binary[i];
}
QInt& QInt::operator=(const QInt& x){
	for (int i = 0; i < 4; i++)
		m_binary[i] = x.m_binary[i];
	return *this;
}

QInt QInt::operator-() const{
	QInt result;
	for (int i = 0; i < 4; i++){
		result.m_binary[i] = ~m_binary[i];
	}

	bool overflow = true;
	for (int i = 0; i < 4 && overflow; i++){
		overflow = result.m_binary[i] == 0xffffffff;
		result.m_binary[i]++;
	}

	return result;
}

bool QInt::GetBit(unsigned short position) const{
	unsigned short block = position / 32;
	unsigned short i = position % 32;
	return (m_binary[block] & (1 << i)) != 0;
}
void QInt::SetBit(unsigned short position, bool on){
	unsigned short block = position / 32;
	unsigned short i = position % 32;
	if (on){
		m_binary[block] |= (1 << i);
	}
	else{
		m_binary[block] &= (~(1 << i));
	}
}

void QInt::ScanQInt(){
	StrInt temp;
	std::cin >> temp;

	bool negative = temp.isNegative();
	unsigned short shift = 0;
	while (!temp.isZero()){
		if (shift >= 128){
			std::cout << "Too big" << std::endl;
			return;
		}
		unsigned short block = shift / 32;
		unsigned short pos = shift % 32;
		if (!temp.isEven()){
			m_binary[block] |= 1 << pos;
		}

		shift++;
		temp = temp.Half();
	}

	if (negative){
		*this = -(*this);
	}
}
void QInt::PrintQInt(){
	QInt temp = *this;
	bool negative = (m_binary[3] & 1 << 31) != 0;
	if (negative){
		temp = -temp;
	}

	StrInt result;
	for (short i = 127; i >= 0; i--){
		unsigned short block = i / 32;
		unsigned short pos = i % 32;

		result = result.Double();
		uint32_t isOn = temp.m_binary[block] & (1 << pos);
		if (isOn != 0)
			++result;
	}

	if (negative){
		result.toNegative();
	}

	std::cout << result << std::endl;
}

bool* QInt::DecToBin(){
	bool* result = new bool[128];

	for (unsigned short shift = 0; shift < 128; shift++){
		unsigned short block = shift / 32;
		unsigned short pos = shift % 32;
		result[shift] = (m_binary[block] & (1 << pos)) != 0;
	}

	return result;
}
QInt QInt::BinToDec(bool* bits){
	QInt result;
	for (unsigned short shift = 0; shift < 128; shift++){
		unsigned short bit = bits[shift] ? 1 : 0;
		unsigned short block = shift / 32;
		unsigned short pos = shift % 32;

		result.m_binary[block] |= bit << pos;
	}

	return result;
}

char* QInt::BinToHex(bool* bits){
	char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	char* result = new char[33];
	for (unsigned short block = 0; block < 32; block++){
		unsigned short total = 0;
		for (short i = 3; i >= 0; i--){
			total = total * 2 + (bits[block * 4 + i] ? 1 : 0);
		}
		result[block] = hex[total];
	}

	result[32] = '\0';

	return result;
}
char* QInt::DecToHex(){
	char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	char* result = new char[33];
	for (unsigned short block = 0; block < 32; block++){
		unsigned short total = 0;
		for (short i = 3; i >= 0; i--){
			total = total * 2 + (GetBit(block * 4 + i) ? 1 : 0);
		}
		result[block] = hex[total];
	}

	result[32] = '\0';

	return result;
}

QInt QInt::operator+(const QInt& q){
	QInt result;
	bool overflow = false;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = m_binary[i] + q.m_binary[i] + (overflow ? 1 : 0);
		if (m_binary[i] < q.m_binary[i]){
			overflow = result.m_binary[i] <= m_binary[i] && result.m_binary[i] < q.m_binary[i];
		}
		else{
			overflow = result.m_binary[i] < m_binary[i] && result.m_binary[i] <= q.m_binary[i];
		}
	}
	return result;
}
QInt QInt::operator-(const QInt& q){
	return *this + (-q);
}

//TODO: * /
QInt QInt::operator*(const QInt& q){
	QInt a, tempQ = q;
	unsigned short temp = 0;
	unsigned short n = 128;
	while (n > 0){
		unsigned short lastQ = tempQ.GetBit(0);
		if (lastQ == 1 && temp == 0){
			a = a - *this;
		}
		else if (lastQ == 0 && temp == 1){
			a = a + *this;
		}
		
		unsigned short lastA = a.GetBit(0);
		a = a >> 1;
		tempQ = tempQ >> 1;
		tempQ.SetBit(127, lastA == 1);
		temp = lastQ;
		n--;
	}
	return tempQ;
}
QInt QInt::operator/(const QInt& q){
	QInt a, temp = *this, tempQ = q;
	unsigned short n = 128;
	bool sameSign = (this->GetBit(127) ^ q.GetBit(127)) == 0;
	if (temp.GetBit(127) == 1){
		temp = -temp;
	}
	if (tempQ.GetBit(127) == 1){
		tempQ = -tempQ;
	}

	while (n > 0){
		unsigned short first = temp.GetBit(127);
		a = a << 1;
		temp = temp << 1;
		a.SetBit(0, first == 1);
		a = a - tempQ;
		if (a.GetBit(127) == 1){
			a = a + tempQ;
		}
		else{
			temp.SetBit(0, true);
		}
		n--;
	}

	if (!sameSign){
		return -temp;
	}
	return temp;
}

QInt QInt::operator&(const QInt& q){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = (m_binary[i] & q.m_binary[i]);
	}
	return result;
}
QInt QInt::operator|(const QInt& q){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = (m_binary[i] & q.m_binary[i]);
	}
	return result;
}
QInt QInt::operator^(const QInt& q){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = (m_binary[i] & q.m_binary[i]);
	}
	return result;
}
QInt QInt::operator~(){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = ~m_binary[i];
	}
	return result;
}

QInt QInt::operator<<(const int n){
	QInt result = *this;
	unsigned short sum = 0;
	while (sum < n){
		unsigned short k;
		if (n - sum > 32){
			k = 32;
		}
		else{
			k = n - sum;
		}
		sum += k;

		uint32_t current = 0;
		uint32_t next = 0;
		for (unsigned short i = 0; i < 4; i++){
			next = 0 | (result.m_binary[i] >> (32 - k));
			result.m_binary[i] = (result.m_binary[i] << k) | current;
			current = next;
		}
	}
	return result;
}
QInt QInt::operator>>(const int n){
	QInt result = *this;
	unsigned short sum = 0;
	while (sum < n){
		unsigned short k;
		if (n - sum > 32){
			k = 32;
		}
		else{
			k = n - sum;
		}
		sum += k;

		uint32_t current = 0;
		uint32_t next = 0;
		for (short i = 3; i >= 0; i--){
			next = 0 | (result.m_binary[i] << (32 - k));
			result.m_binary[i] = (result.m_binary[i] >> k) | current;
			current = next;
		}
		if (GetBit(127)){
			result.m_binary[3] |= 0xffffffff << (32 - k);
		}
	}
	return result;
}
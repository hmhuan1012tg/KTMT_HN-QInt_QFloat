#pragma once
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

const int AFTER_DOT = 35;												//Số chữ số tối đa sau dấu phẩy ở cơ số 10: 2^-112 ~ 10^-35
class StrFloat															//Lưu số Float ở cơ số 10. //Save Float at base 10
{
	string m_float;														//Lưu dãy số.
	bool m_negative;													//Lưu đấu. m_negative=true (âm), =false (dương)
public:
	StrFloat();															//Khởi tạo giá trị ban đầu. 
	StrFloat(string p);													//Chuyển một chuỗi vào số.
	~StrFloat();
	bool isNegative();													//Kiểm tra số âm.
	void normalize();													//Chuẩn hóa: loại bỏ "-" trong chuỗi, số lượng số sau dấu phẩy = AFTER_DOT, xóa số 0 ở đầu, kiểm tra trường hợp "-0".

	StrFloat operator+(const StrFloat &p);
	StrFloat& operator=(StrFloat &p);
	StrFloat& operator/(int p);
	StrFloat& operator*(int p);

	StrFloat operator/=(int p);
	StrFloat operator+=(const StrFloat& p);
	StrFloat operator++(int x);

	friend istream& operator>>(istream& is, StrFloat& p);
	friend ostream& operator<<(ostream& os, const StrFloat &p);
};

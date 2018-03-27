//QFloat.h

#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include "StrFloat.h"
using namespace std;

const int MAX_VALUE_EXP = 16383; //2^14-1=16383_Giá trị lớn nhất của mũ
const int MIN_VALUE_EXP = -(MAX_VALUE_EXP + 1); //-16382_Giá trị nhỏ nhất của mũ
const int LENGTH_OF_AFTER_POINT_BASE_2 = 112; //Tương đương với 112 bit biểu diễn giá trị trong QFloat
class QFloat
{
	int m_el[4]; //el: element_ lưu dãy 128 bit dưới dạng số thập phân. //Save 128 bits under DEC form.
				//el[0]: bit 0 - 31
				//el[1]: bit 32 - 63
				//el[2]: bit 64 - 95
				//el[3]: bit 96 - 127
public:
	QFloat(); //Khởi tạo giá trị bằng 0 cho phần tử của el. // m_el[i] = 0 (i=0..3)
	~QFloat();

	bool GetBit(unsigned short position); //Lấy giá trị của bit. // Get value of bit at position
	void SetBit(unsigned short position, bool on); //Đặt giá trị của bit. Set // value of bit at position
	bool CheckBitScope(int l, int r, int sd); //Kiểm tra dãy bit từ l đến r sao cho mỗi bit bằng sd. // Check the bit's scope between l and r whether each bit equals to sd (standard)
	int GetTypeOfQFloat(); //Kiểm tra loại của QFloat, trả về các giá trị: // Check type of QFloat, return value
							//0: Số 0 // Number 0
							//1: Dạng chuẩn // Normalized
							//2: Dạng không chuẩn // Denormalized
							//3: Vô cùng // Infinite
							//4: Lỗi // NaN

	void ScanQFloat(int base);//Đọc số QFloat, với base là cơ số. // Scan QFloat with base is form of the number input
	void PrintQFloat(); //In số QFloat ở dạng cơ số 10. //Print QFloat at DEC

	void convertToQFloat(string a, int N_exp); //Chuyển dãy nhị phân a với số mũ là N_exp về dạng QFloat. // Convert bit string a with value exponent N_exp to QFloat
	QFloat BinToDec(bool *bit); // el[0]: 0-31; el[1]:32-63; el[2]:64-95; el[3]:96-127
	bool * DecToBin(); //  1-128: el[0] -> el[3]
};

//QFloat.cpp

#include "QFloat.h"


void convertBFD(string &bfd)
{
	string res = "";
	string tmp = bfd;
	int j = 0;
	int sign = 0;
	if (bfd[0] == '-') {
		sign = -1;
		bfd.erase(0, 1);
	}
	while (tmp.length() != 0) {
		j = j * 10 + tmp[0];
		res.push_back(j % 2 + '0');
		j = j / 2;
		tmp.erase(0, 1);
	}
	if (sign == -1) res.push_back('-');
	res.reserve(res.length());
	bfd = res;
}
int checkx10(string &a)
{
	int posx10 = -1;
	int  i = 0;
	for (posx10 = a.length() - 1; posx10 >= 0; posx10--)
		if (a[posx10] == 'x') break;
	if (posx10 == -1) return;
	a.erase(posx10 + 1, 3);
	int sign = 1;
	if (a[posx10 + 1] == '-') {
		sign = -1;
		a.erase(posx10 + 1, 1);
	}
	int vx10 = 0;
	while (a.length() > posx10)
	{
		vx10 = vx10 * 10 + a[posx10 + 1] - '0';
		a.erase(posx10 + 1, 1);
	}
	return vx10*sign;
}
int convertAFD(string &afd, int Nexp)
{
	int  j = 0;
	string res = ".";
	while (res.length()-1 <112-Nexp) {
		int s = 0;
		j = 0;
		for (int i = afd.length() - 1; i >= 0; i--)
			if (afd[i]!='.')
			{
				j = (afd[i] - '0') * 2+s;
				if (j > 9) s = 1;
				else s = 0;
				j %= 10;
				afd[i] = j + '0';
			}
		res.push_back(afd[0]);
		afd.erase(0, 1);
		afd.insert(0, 1, '0');
	}
	int res1 = 0;
	while (j > 0 && res1 + res.length() + Nexp < 16383) //2^14-1=16383
	{
		int s = 0;
		j = 0;
		for (int i = afd.length() - 1; i >= 0; i--)
			if (afd[i] != '.')
			{
				j = (afd[i] - '0') * 2 + s;
				if (j > 9) s = 1;
				else s = 0;
				j %= 10;
				afd[i] = j + '0';
			}
		res1++;
		afd.erase(0, 1);
		afd.insert(0, 1, '0');
	}
	afd = res;
	return res1;
}
int convertToBin(string &a)
{
	int hx10=checkx10(a);
	string bfd = "";
	string afd = "0.";
	int i;
	//Extract numbers before dot
	for (i = 0; i < a.length() && a[i] != '.'; i++)
		bfd.push_back(a[i]);
	//Extract numbers after dot
	i++;
	for (i = i; i < a.length(); i++)
		afd.push_back(a[i]);
	afd.push_back('0');
	if (hx10 > 0) {
		for (i = 1; i <= hx10; i++)
		{
			bfd.push_back(afd[2]);
			afd.erase(2, 1);
			afd.push_back('0');
		}
	}
	else
	{
		for (i = 1; i <= hx10; i++)
		{
			afd.insert(2, 1, bfd[bfd.length() - 1]);
			bfd.erase(bfd.length() - 1, 1);
			if (bfd.length() ==0 || bfd[bfd.length()-1]=='-') bfd.push_back('0');
		}
	}
	convertBFD(bfd);

	int Nexp = bfd.length()-1-(bfd[0]=='-'); //The number can add to express
	int N_exp=convertAFD(afd, Nexp);
	a = bfd + afd;
	return N_exp;
}
void normalizeBin(string &a,int &N_exp)
{
	string bfd = "", afd = ".";
	int i;
	for (i = 0; i < a.length() && a[i] != '.'; i++) bfd.push_back(a[i]);
	for (i = i + 1; i < a.length();i++) afd.push_back(a[i]);
	while (bfd.length() > 1)
	{
		if (bfd.length() == 3 && bfd[0] = '-') break;
		afd.insert(1, 1, bfd[bfd.length()]);
		bfd.erase(bfd.length() - 1);
		N_exp++;
	}
	while (afd.length() < 128) afd.push_back('0');
	while (afd.length() > 128) afd.erase(afd.length() - 1, 1);
	a = bfd + afd;
}
//____________________Class______________________________//
//Lấy giá trị của bit. // Get value of bit at position
bool QFloat::GetBit(unsigned short position) { 
	unsigned short block = position / 32; //Tính xem vị trí position nằm ở ô nào trong m_el.// Calculate block which postition in. 
	unsigned short i = position % 32; //Tính bit thứ mấy trong block. // Calculate the order of bit in block.
	// Giải thích: Vì mỗi m_el[i] lưu 32 bit và có tất cả 128 bit nên ta dùng hai phép trên để xác định vị trí tương đối của bit cần lấy trong m_el
	// Explain: Each m_el[i] saves 32 bits and there are 128 bit so we use two expressions to identify relative position of bit in m_el
	return (m_el[block] & (1 << i)) != 0;
}
//Đặt giá trị của bit. Set // value of bit at position 
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

//Đọc số QFloat, với base là cơ số. // Scan QFloat with base is form of the number input
void QFloat::ScanQFloat(int base)
{
	//If base==10, turn it into binary with 35 number after dot. If base==2, normalize it. After all, converting to QFloat.
	int N_exp = 0;
	string tmp;
	cin >> tmp;
	if (base == 10) 
		N_exp=convertToBin(tmp);
	normalizeBin(tmp,N_exp);
	this->convertToQFloat(tmp, N_exp);
}

//Kiểm tra dãy bit từ l đến r sao cho mỗi bit bằng sd. // Check the bit's scope between l and r whether each bit equals to sd (standard)
bool QFloat::CheckBitScope(int l, int r,int sd) // l=begining of bit'scope, r=end of bit's scope, sd(means standard)=value of each bit needs checking (only 0 or 1)
{
	for (int i = l; i <= r; i++)
		if (GetBit(i) != sd) return false;
	return true;
}

//Kiểm tra loại của QFloat, trả về các giá trị: // Check type of QFloat, return value
//0: Số 0 // Number 0
//1: Dạng chuẩn // Normalized
//2: Dạng không chuẩn // Denormalized
//3: Vô cùng // Infinite
//4: Lỗi // NaN
int QFloat::GetTypeOfQFloat() //
{
	int exp = 1; //exp=0 means that numver 0; 
				 //exp=1 means standard number;
				 //exp=2 means denormalized number
				//exp=3 means Infinite
				//exp=4 means NaN
	if (CheckBitScope(1, 15, 1)) //Kiểm tra bit dấu (bit từ 1 đến 15) có bằng 1. -> đúng thì kiểm tra loại Infinite hoặc NaN.
	{
		if (CheckBitScope(16, 127, 0)) exp = 3; // cout << "Infinite";
		else exp = 4; //cout << "NaN";
	}
	else
		if (CheckBitScope(1, 15, 0)) //Kiểm tra bit dấu (bit từ 1 đến 15) có bằng 0.  -> Đúng thì kiểm tra số 0 hoặc dạng không chuẩn.
		{
			if (CheckBitScope(16, 127, 0)) exp = 0; //Kiểm tra bit giá trị (bit từ 16 đến 127) có bằng 0. -> Đúng là số 0, sai là dạng không chuẩn.
			else exp = 2;
		}
	return exp;
}

//In số QFloat ở dạng cơ số 10. //Print QFloat at DEC
void QFloat::PrintQFloat()
{
	//Lấy loại của QFloat. 
	int exp = GetTypeOfQFloat();
	//double value; //contains the value of bit after dot.  After finishing StrFloat, (*) convert into StrFloat 
	StrFloat value("0.0"); //Lưu giá trị của các bit sau dấu phẩy ở dạng thập phân.

	//Kiểm tra loại của QFloat, trả về các giá trị: // Check type of QFloat, return value
	//0: Số 0 // Number 0
	//1: Dạng chuẩn // Normalized
	//2: Dạng không chuẩn // Denormalized
	//3: Vô cùng // Infinite
	//4: Lỗi // NaN
	if (exp == 0) cout << 0;
	else
		if (exp == 3) cout <<  (CheckBitScope(0, 0, 1)==true ? "-" : "") << "Infinite";
		else
			if (exp == 4) cout << "NaN";
			else
			{
				int sign = CheckBitScope(0, 0, 1); //0: (+); 1:(-)
				int base2 = 1; //Biến lưu giá trị tạm để tính 2^(x). // temporary variable constain 2^(x)
				//double base2after = 1; //temporary variable constain 2^(-x) (*)
				StrFloat base2after("1"); //Biến lưu giá trị tạm để tính 2^(-x). //temporary variable constain 2^(-x) (*)
				int Vexp = 0; //Lưu giá trị của mũ // Value of exp
				int i;
				
				//Tính giá trị của mũ theo số bít lưu trong QFloat
				for (i = 15; i > 0; i--) {
					if (GetBit(i)) Vexp += base2;
					base2 *= 2;
				}
				//Chuyển lại theo số Bias bằng cách trừ đi lượng MAX_VALUE_EXP -1
				Vexp -= (MAX_VALUE_EXP - 1) -1 ; // (base2 / 2 - 1);

				//Chuyển đối giá trị sau dấu phẩy về dạng thập phân với 35 số sau dấu phẩy.
				for (i = 16; i < 128; i++){
					base2after /= 2; //(*)
					if (GetBit(i)) value += base2after; //(*)
				}
				//Nếu là dạng chuẩn thì cộng thêm 1.
				if (exp == 1) value++; //(*)

				//In giá trị
				if (sign) cout << '-'; //Kiểm tra số âm.
				cout << value << "x2^" << Vexp;
			}
}

//Chuyển dãy nhị phân a với số mũ là N_exp về dạng QFloat. // Convert bit string a with value exponent N_exp to QFloat
void QFloat::convertToQFloat(string a, int N_Exp)
{
	int i;
	//Sign
	this->SetBit(0, a[0] == '-'); //Nếu a[0]=='-' thì đặt bit đầu là 1, ngược lại là 0.
	if (a[0] == '-') a.erase(0, 1); //Xóa kí tự đầu tiên của a nếu là '-'

	//Chia ra theo loại biểu diễn nhị phân để chuyển đổi.
	if (a[0] == '1') //Dãy nhị phân a ở dạng 1.xxxx với số mũ là N_Exp.
	{
		if (N_Exp > MAX_VALUE_EXP) //Infinitite //Lớn hơn phạm vi mà QFloat có thể biểu diễn ở dạng nhị phân. (số mũ > MAX_VALUE_EXP)
		{
			for (i = 1; i < 16; i++) this->SetBit(i, 1); // Đặt tất cả các bit mũ = 1. 
			for (i = 16; i < 128; i++) this->SetBit(i, 0); // Đặt tất cả các bit giá trị = 0.
		}
		if (N_Exp <= MIN_VALUE_EXP) //NaN //Số mũ nhỏ hơn hoặc bằng MIN_VALUE_EXP báo số lỗi. ???(Liệu có chuyển về được dạng 0.xxx với mũ N_exp=MIN_VALUE_EXP)
		{
		NaN:
			for (i = 1; i < 16; i++) this->SetBit(i, 1); // Đặt bit mũ =1.
			// Đặt bit giá trị = 0 và bit thứ 16 = 1
			for (i = 17; i < 128; i++) this->SetBit(i, 0);
			this->SetBit(17, 1);
		}
		else
		{ //Normalized
		Normalized:
			// Chuyển số mũ về dạng Bias bằng cách cộng cho 2^14-1.
			N_Exp += MAX_VALUE_EXP-1;
			for (i = 15; i > 0; i--) {
				this->SetBit(i, N_Exp % 2);
				N_Exp /= 2;
			}
			//Gán những bit sau dấu phẩy trong a vào QFloat.
			int j = 2;
			for (i = 16; i < 128; i++)
				this->SetBit(i, a[j++] == '1');
		}
	}
	else
	{ //Denormalized
		//Chuyển từ dạng 0.xxx về 1.xxx với số mũ > -(2^14-1) = -16382 = MIN_VALUE_EXP
		//						hoặc dạng 0.xxxx với số mũ = MIN_VALUE_EXP
		// Nếu không thì dừng lại.
		while (a[0] == '0' && N_Exp > MIN_VALUE_EXP)
		{
			N_Exp--;
			a.erase(0, 2);
			a.insert(1, 1, '.');
			a.push_back('0');
		}
		//Nếu ở dạng 0.xxx với số mũ MIN_VALUE_EXP hoặc 1.xx với số mũ > MIN_VALUE_EXP. Ngược lại thì nằm ngoài phạm vi biểu diễn.
		if ( (a[0]=='0' && N_Exp == MIN_VALUE_EXP) || (a[0]=='1' && N_Exp>MIN_VALUE_EXP)) goto Normalized;
		else goto NaN;
	}
}

// el[0]: 0-31; el[1]:32-63; el[2]:64-95; el[3]:96-127
QFloat QFloat::BinToDec(bool * bit)
{
	//Chuyển từ dãy bit -> QFloat p. //Convert array bit to QFloat p.
	QFloat p;
	for (int i = 0; i < 128; i++)
		p.SetBit(i, bit[i]);
	return p;
}

//  1-128: el[0] -> el[3]
bool * QFloat::DecToBin()
{
	//Chuyển từ el[i] vào mảng p lưu giá trị nhị phân. 0: false; 1: true. //Convert e[i] to array bool p with 0: false, 1: true.
	bool *p;
	p = new bool[128];
	for (int i = 0; i < 128; i++)
		p[i] = this->GetBit(i);
	return p;
}

//Khởi tạo giá trị bằng 0 cho phần tử của el. // m_el[i] = 0 (i=0..3)
QFloat::QFloat()
{
	for (int i = 0; i < 3; i++)
		m_el[i] = 0;
}

QFloat::~QFloat()
{
}

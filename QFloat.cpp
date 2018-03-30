//QFloat.cpp

#include "QFloat.h"

//Chia bfd cho 2.
void div2(string &bfd)
{
	int p = 2, j = 0;
	string tmp = "";
	for (int k = 0; k < bfd.length(); k++) //Thực hiện phép chia từ trái sang phải.
	{
		j = j * 10 + bfd[k] - '0'; //Lấy số thứ k trong chuỗi.
		tmp.push_back((j / p) + '0'); //Lưu kết quả vào sau tmp.
		j %= p;
	}
	while (tmp.length() > 1 && tmp[0] == '0') tmp.erase(0, 1);
	bfd = tmp;
}

//Chuyển về dạng nhị phân những số trước dấu phẩy bằng cách chia cho 2.
void convertBFD(string &bfd)
{
	string res = ""; //Lưu tạm giá trị phép dư số khi chia cho 2.
	string tmp = bfd; //Lưu tạm giá trị của bfd. 
	int j = 0;
	int sign = 0; //Lưu giá trị dấu
	if (tmp[0] == '-') { //Kiểm tra dấu
		sign = -1;
		tmp.erase(0, 1); //Xóa dấu
	}

	//Thực hiện phép chia 2, lưu lại số dư trong từng phép chia.
	while (tmp.length() != 0 && tmp[0] != '0') {
		if ((tmp[tmp.length() - 1] - '0') % 2 == 1) res.push_back('1');
		else res.push_back('0');
		div2(tmp);
	}

	if (res.length() == 0) res.push_back('0');

	//Thêm dấu cho kết quả ở dạng nhị phân.
	if (sign == -1) res.push_back('-');

	//Chuyển đổi về đúng dạng nhị phân bằng cách đảo chuỗi.
	reverse(res.begin(), res.end());

	//Gán lại kết quả.
	bfd = res;
}

//Kiểm tra dãy số nhập vào có dạng xxxxx*10^x hay không? Nếu có thì chuyển a về dạng tự nhiên, nghĩa là dịch dấu phẩy về trái hoặc phải.
int checkx10(string &a)
{
	int posx10; //Ví trí xuất hiện kí tự: 'x'
	for (posx10 = a.length() - 1; posx10 >= 0; posx10--) //Chạy từ cuối về kiểm tra.
		if (a[posx10] == 'x') break;
	//Không tìm thấy thì thoát.
	if (posx10 == -1) return 0;

	a.erase(posx10 + 1, 3); //Xóa chuỗi "10^" từ (vị trí "x" +1) đến (vị trí "x" +3)
	int sign = 1; //Lưu dấu của giá trị mũ 
	if (a[posx10 + 1] == '-') {//Kiểm tra mũ âm
		sign = -1;
		a.erase(posx10 + 1, 1); //Xóa mũ âm.
	}

	int vx10 = 0; //Lưu giá trị của mũ.
	while (a.length() > posx10 + 1) //Xóa khi còn kí tự 'x'
	{
		vx10 = vx10 * 10 + a[posx10 + 1] - '0'; //Tính giá trị của mũ
		a.erase(posx10 + 1, 1);
	}
	//Xóa dấu 'x'
	a.erase(a.length() - 1, 1);
	//Trả về giá trị kèm với dấu.
	return vx10*sign;
}

//Chuyển về dạng nhị phân của những số sau dấu phẩy bằng cách nhân 2 rồi lấy phần nguyên. 
void convertAFD(string &afd, int Nexp) //Nexp: lưu số lượng số có thể chuyển từ bfd sang cho afd để thay đổi số mũ N_exp
{
	int  j;
	string res = "."; //Lưu giá trị số sau dấu phẩy phần thập phân ở dạng cơ số 2. Độ dài tối đa là 112 bit nhưng do chuyển từ bfd sang Nexp số nên chiều dài của res= 112-Nexp
	while (res.length() - 1 < LENGTH_OF_AFTER_POINT_BASE_2 - Nexp) { //-1 vì có '.'
		//Nhân afd với 2.
		int s = 0; // lưu số nhớ trong phép nhân.
		j = 0; //Lưu phần trích của số để nhân.
		for (int i = afd.length() - 1; i >= 0; i--)
			if (afd[i] != '.')
			{
				j = (afd[i] - '0') * 2 + s;
				if (j > 9) s = 1;
				else s = 0;
				j %= 10;
				afd[i] = j + '0';
			}
		res.push_back(afd[0]); //Thêm vào sau kết quả của phần nguyên afd sau khi nhân.
		afd.erase(0, 1); //Bỏ phần nguyên của afd.
		afd.insert(0, 1, '0'); //Thêm '0' vào phần nguyên của afd.
	}
	afd = res;
}
int convertToBin(string &a)
{
	int hx10 = checkx10(a); //Kiểm tra xem a có dạng x10^
	string bfd = ""; //Lưu số trước dấu phẩy.
	string afd = "0."; //Lưu số sau dấu phẩy.
	int i;
	//Tách số trước dấu phẩy. //Extract numbers before dot
	for (i = 0; i < a.length() && a[i] != '.'; i++)
		bfd.push_back(a[i]);
	//Tách số sau dấu phẩy. // Extract numbers after dot
	i++; //Nhảy qua "."
	for (i = i; i < a.length(); i++)
		afd.push_back(a[i]);
	afd.push_back('0');//Tránh trường hợp "0." nên cần thêm "0" vào sau cùng (không thay đổi giá trị của số)

	//Nếu a có dạng "x10^" thì chuyển theo từng trường hợp (>0 hoặc <0)
	if (hx10 > 0) {
		for (i = 1; i <= hx10; i++)
		{
			bfd.push_back(afd[2]); //afd[2] bỏ qua "0."
			afd.erase(2, 1);
			afd.push_back('0'); //Thêm 0 vào để luôn có số sau dấu phẩy.
		}
	}
	else
	{
		for (i = 1; i <= hx10; i++)
		{
			afd.insert(2, 1, bfd[bfd.length() - 1]); //Chuyển số cuối từ bfd sang làm số đầu của afd.
			bfd.erase(bfd.length() - 1, 1);
			if (bfd.length() == 0 || bfd[bfd.length() - 1] == '-') bfd.push_back('0'); //Xét trường hợp bfd rỗng hoặc bfd="-".
		}
	}

	convertBFD(bfd); //Chuyển đổi bfd
	int Nexp = bfd.length() - 1 - (bfd[0] == '-'); //Số lượng số có thể chuyển sang cho afd với điều kiện bfd chứ 1 số. // The number can add to express
	convertAFD(afd, Nexp);

	a = bfd + afd;
	return Nexp;
}
void normalizeBin(string &a)
{
	string bfd = "", afd = "."; //bfd: lưu số trước dấu phẩy 
								//afd: lưu số sau dấu phẩy
	int i;
	//Tách thành 2 phần: Trước dấu phẩy và sau dấu phẩy.
	for (i = 0; i < a.length() && a[i] != '.'; i++) bfd.push_back(a[i]); //Tách trước dấu phẩy.
	for (i = i + 1; i < a.length(); i++) afd.push_back(a[i]); //Tách sau dấu phẩy.

	while (bfd.length() > 1) //Chuyển về dạng x.xxxxx tức là bfd có 1 kí tự duy nhất. 
	{
		if (bfd.length() == 2 && bfd[0] == '-') break; //Dừng khi bfd="-x"
		afd.insert(1, 1, bfd[bfd.length() - 1]); //Thêm số vào sau dấu phẩy. (Thêm vào đầu afd)
		bfd.erase(bfd.length() - 1); //Xóa số trước dấu phẩy. (Xóa cuối bfd).
	}

	//Làm cho số lượng số sau dấu phẩy bằng LENGTH_OF_AFTER_POINT_BASE_2 (=112 là số bit biểu diễn giá trị)
	while (afd.length() - 1 < LENGTH_OF_AFTER_POINT_BASE_2) afd.push_back('0'); //Thêm cho đủ
	while (afd.length() - 1 > LENGTH_OF_AFTER_POINT_BASE_2) afd.erase(afd.length() - 1, 1); //Nếu dư thì xóa
	a = bfd + afd; //Nối lại để thành chuỗi hoàn chỉnh.
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
//Đặt giá trị của bit. //Set value of bit at position 
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

//Chuyển dãy nhị phân a với số mũ là N_exp về dạng QFloat. // Convert bit string a with value exponent N_exp to QFloat
void QFloat::convertToQFloat(string a, int N_Exp)
{
	int i;
	//Dấu //Sign
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
			// Chuyển số mũ về dạng Bias bằng cách cộng cho MAX_VALUE_EXP. Nếu là dạng không chuẩn (N_exp=0) thì cộng thêm MAX_VALUE_EXP-1.
			if (N_Exp != MIN_VALUE_EXP) N_Exp += (MAX_VALUE_EXP);
			else N_Exp += MAX_VALUE_EXP - 1;

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
	{  //Denormalized
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
		if ((a[0] == '0' && N_Exp == MIN_VALUE_EXP) || (a[0] == '1' && N_Exp > MIN_VALUE_EXP)) goto Normalized;
		else goto NaN;
	}
}

//Đọc số QFloat, với base là cơ số. // Scan QFloat with base is form of the number input
void QFloat::ScanQFloat(int base)
{
	//Nếu là ở dạng cơ số 10 (base = 10) thì chuyển về dạng nhị phân với MAX_VALUE_EXP số sau dấu phẩy. Chuẩn hóa dạng nhị phân và chuyển về QFloat. //If base==10, turn it into binary with MAX_VALUE_EXP number after dot. Normalize it and converting to QFloat.
	int N_exp = 0; //Lưu số lượng chữ số đứng trước dấu phẩy, phục vụ cho việc tính toán dời dấu phẩy về đằng trước.
	string tmp;
	cin >> tmp;
	if (base == 10)
		N_exp = convertToBin(tmp); //Đổi về cơ số 2.
	normalizeBin(tmp); //Chuẩn hóa ở dạng cơ số 2.
	this->convertToQFloat(tmp, N_exp); //Chuyển về dạng QFloat.
}

//Kiểm tra dãy bit từ l đến r sao cho mỗi bit bằng sd. // Check the bit's scope between l and r whether each bit equals to sd (standard)
bool QFloat::CheckBitScope(int l, int r, int sd) // l=begining of bit'scope, r=end of bit's scope, sd(means standard)=value of each bit needs checking (only 0 or 1)
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
		if (exp == 3) cout << (CheckBitScope(0, 0, 1) == true ? "-" : "") << "Infinite" << endl;
		else
			if (exp == 4) cout << "NaN" << endl;
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
				//Chuyển lại theo số Bias bằng cách trừ đi lượng MAX_VALUE_EXP nếu là số dạng không chuẩn thì trừ đi MAX_VALUE_EXP – 1.
				if (Vexp != 0) Vexp -= (MAX_VALUE_EXP);
				else Vexp -= (MAX_VALUE_EXP - 1);

				//Chuyển đối giá trị sau dấu phẩy về dạng thập phân với 35 số sau dấu phẩy.
				for (i = 16; i < 128; i++) {
					base2after /= 2; //Tính 2^(-x) x=i-15
					if (GetBit(i))
						value += base2after; //Nếu giá trị của bit = 1 thì cộng dồn vào giá trị sau dấu phẩy.
				}
				if (exp == 1) value++;

				//In giá trị
				//if (sign) cout << '-'; //Kiểm tra số âm.
				//cout << value << "x2^" << Vexp << endl; //Xuất ra dạng x.xxxx*2^x
				if (sign) cout << '-';
				if (Vexp > 0) for (i = 0; i < Vexp; i++) value = value * 2;
				else
					if (Vexp < 0) for (i = 0; i < -Vexp; i++) value = value / 2;
				cout << value << endl;
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

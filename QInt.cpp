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

QInt operator-(QInt q){
	QInt result;
	for (int i = 0; i < 4; i++){
		result.m_binary[i] = ~q.m_binary[i];
	}

	bool overflow = true;
	for (int i = 0; i < 4 && overflow; i++){
		overflow = result.m_binary[i] == 0xffffffff;
		result.m_binary[i]++;
	}

	return result;
}

// Lấy bit ở vị trí position
bool GetBit(QInt q, unsigned short position){
	unsigned short block = position / 32; // Block data cần chọn
	unsigned short i = position % 32; // Vị trí cần lấy trong block data
	return (q.m_binary[block] & (1 << i)) != 0;
}
// Đặt bit ở vị trí position, đặt = 1 nếu on = true, nếu không đặt = 0
void SetBit(QInt& q, unsigned short position, bool on){
	unsigned short block = position / 32;
	unsigned short i = position % 32;
	if (on){
		q.m_binary[block] |= (1 << i);
	}
	else{
		q.m_binary[block] &= (~(1 << i));
	}
}

// Đọc vào QInt dưới dạng số thập phân từ stdin
void ScanQInt(QInt& q){
	// Đọc vào StrInt
	StrInt temp;
	std::cin >> temp;

	bool negative = temp.isNegative(); // Kiểm tra âm
	unsigned short shift = 0;
	// Chia 2 cho đến khi = 0
	while (!temp.isZero()){
		// Nếu vượt quá số bit có, thông báo số lớn
		if (shift >= QInt::SIZE_OF_QINT){
			std::cout << "Too big" << std::endl;
			return;
		}
		
		// Nếu số chẵn, set bit = 0, nếu lẻ set bit = 1
		unsigned short block = shift / 32;
		unsigned short pos = shift % 32;
		if (!temp.isEven()){
			q.m_binary[block] |= 1 << pos;
		}

		shift++;
		temp = temp.Half();
	}

	// Nếu âm, đổi dấu
	if (negative){
		q = -q;
	}
}

// In QInt ra stdout dưới dạng thập phân
void PrintQInt(QInt q){
	bool negative = (q.m_binary[3] & 1 << 31) != 0;
	// Nếu âm, đổi thành dương
	if (negative){
		q = -q;
	}
	
	// Tạo biến StrInt chứa giá trị thập phân
	StrInt result;
	// Đi từ trái qua, nhân 2 tuần tự, gặp bit 1 thì cộng thêm 1
	for (short i = 127; i >= 0; i--){
		unsigned short block = i / 32;
		unsigned short pos = i % 32;

		result = result.Double();
		uint32_t isOn = q.m_binary[block] & (1 << pos);
		if (isOn != 0)
			++result;
	}
	
	// Nếu âm, đổi StrInt thành âm
	if (negative){
		result.toNegative();
	}

	std::cout << result << std::endl;
}

// Chuyển thập phân sang nhị phân
std::string DecToBin(QInt q){
	std::string result;
	
	// Lấy liên tục các bit từ trái qua thêm vào chuỗi
	unsigned short prevBit = GetBit(q, 127);
	unsigned short curBit = GetBit(q, 126);
	short pos = 126;
	while (curBit == prevBit && pos >= 0){
		prevBit = curBit;
		pos--;
		curBit = GetBit(q, pos);
	}
	pos++;

	while (pos >= 0){
		result.push_back(GetBit(q, pos--) + '0');
	}

	return result;
}
// Chuyển từ nhị phân sang thập phân
QInt BinToDec(const std::string& bits){
	QInt result;
	short length = bits.length();
	// Set bit theo chuỗi nhị phân
	for (short i = 0; i < length; i++){
		SetBit(result, length - 1 - i, bits[i] == '1');
	}
	// Set các bit đầu dựa trên bit đầu của chuỗi nhị phân
	for (short i = length; i < QInt::SIZE_OF_QINT; i++){
		SetBit(result, i, bits[0] == '1');
	}

	return result;
}

// Chuyển từ nhị phân sang thập lục phân
std::string BinToHex(const std::string& bits){
	// Mảng thập lục phân cơ bản
	char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	std::string result;
	std::string temp;

	unsigned short pos = 0;
	unsigned short total = 0;
	unsigned short multiplier = 1; // Biến nhân
	while (pos < bits.length()){
		
		// Chạy từ trái qua phải, gặp bit 1 thì cộng thêm 2^multiplier vào total
		// multiplier * 2 mỗi lần chạy
		total = total + (bits[bits.length() - 1 - pos] - '0') * multiplier;
		pos++;
		multiplier *= 2;
		
		// Nếu đã đủ số 8 bit -> chuyển total thành thập lục phân = mảng đã tạo
		if (pos % QInt::BIN_PER_HEX == 0){
			temp.push_back(hex[total]);
			total = 0;
			multiplier = 1;
		}
	}
	
	// Nếu chưa đủ bit cho 1 thập lục phân, add thêm các bit = bit đầu
	while (pos % QInt::BIN_PER_HEX != 0){
		total = total + (bits[0] - '0') * multiplier;
		pos++;
		multiplier *= 2;
		if (pos % QInt::BIN_PER_HEX == 0){
			temp.push_back(hex[total]);
		}
	}

	for (short i = temp.length() - 1; i >= 0; i--){
		result.push_back(temp[i]);
	}

	return result;
}

// Chuyển thập phân sang thập lục phân
std::string DecToHex(QInt q){
	return BinToHex(DecToBin(q));
}
// Chuyển thập lục phân sang thập phân
QInt HexToDec(const std::string& hexs){
	QInt result;

	uint32_t multiplier = 1; // biến nhân 16^x
	for(unsigned short pos = 0; pos < hexs.length(); pos++){
		// chạy từ phải qua, tính đủ số thập lục phân cho 1 block data
		// rồi reset lại
		char c = hexs[hexs.length() - 1 - pos];
		unsigned short val = c < 'A' ? (c - '0') : (10 + c - 'A');
		unsigned short block = pos / QInt::HEX_PER_UINT;
		result.m_binary[block] = result.m_binary[block] + (val * multiplier);
		multiplier *= 16;
	}

	//if (hexs[0] >= '8'){
	//	unsigned short block = (hexs.length() - 1) / QInt::HEX_PER_UINT;
	//	unsigned short shift = ((hexs.length() - 1) % QInt::HEX_PER_UINT + 1) * QInt::BIN_PER_HEX;
	//	result.m_binary[block] |= 0xffffffff << shift;
	//	for (unsigned short i = block + 1; i < QInt::UINT_NUM; i++){
	//		result.m_binary[i] = 0xffffffff;
	//	}
	//}

	return result;
}

// Phép cộng
QInt operator+(QInt q, QInt m){
	QInt result;
	bool overflow = false;
	for (unsigned short i = 0; i < 4; i++){
		// Cộng bình thường các block data
		// Nếu có overflow từ trước, cộng thêm 1
		result.m_binary[i] = q.m_binary[i] + m.m_binary[i] + (overflow ? 1 : 0);
		overflow = 0xffffffff - q.m_binary[i] < m.m_binary[i];
	}
	return result;
}
// Phép trừ
QInt operator-(QInt q, QInt m){
	return q + (-m);
}
// Phép nhân thuật toán booth
QInt operator*(QInt q, QInt m){
	QInt a;
	unsigned short temp = 0;
	unsigned short n = QInt::SIZE_OF_QINT;
	while (n > 0){
		unsigned short lastQ = GetBit(m, 0);
		if (lastQ == 1 && temp == 0){
			a = a - q;
		}
		else if (lastQ == 0 && temp == 1){
			a = a + q;
		}
		
		unsigned short lastA = GetBit(a, 0);
		a = a >> 1;
		m = m >> 1;
		SetBit(m, 127, lastA == 1);
		temp = lastQ;
		n--;
	}
	return m;
}
// Phép chia
// Chia không dấu
// Nếu 2 số khác dấu
// Thì đổi dấu thương
QInt operator/(QInt q, QInt m){
	QInt a;
	unsigned short n = QInt::SIZE_OF_QINT;
	bool sameSign = (GetBit(q, 127) ^ GetBit(m, 127)) == 0;
	if (GetBit(q, 127) == 1){
		q = -q;
	}
	if (GetBit(m, 127) == 1){
		m = -m;
	}

	while (n > 0){
		unsigned short first = GetBit(q, 127);
		a = a << 1;
		q = q << 1;
		SetBit(a, 0, first == 1);
		a = a - m;
		if (GetBit(a, 127) == 1){
			a = a + m;
		}
		else{
			SetBit(q, 0, true);
		}
		n--;
	}

	if (!sameSign){
		return -q;
	}
	return q;
}

// Phép AND
QInt operator&(QInt q, QInt m){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = (q.m_binary[i] & m.m_binary[i]);
	}
	return result;
}
// Phép OR
QInt operator|(QInt q, QInt m){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = (q.m_binary[i] | m.m_binary[i]);
	}
	return result;
}
// Phép XOR
QInt operator^(QInt q, QInt m){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = (q.m_binary[i] ^ m.m_binary[i]);
	}
	return result;
}
// Phép NOT
QInt operator~(QInt q){
	QInt result;
	for (unsigned short i = 0; i < 4; i++){
		result.m_binary[i] = ~q.m_binary[i];
	}
	return result;
}

// Dịch arithmetic trái
QInt operator<<(QInt q, const int n){
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
			next = 0 | (q.m_binary[i] >> (32 - k));
			q.m_binary[i] = (q.m_binary[i] << k) | current;
			current = next;
		}
	}
	return q;
}
// Dịch arithmetic phải
QInt operator>>(QInt q, const int n){
	unsigned short sum = 0;
	bool negative = GetBit(q, 127) == 1;
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
			next = 0 | (q.m_binary[i] << (32 - k));
			q.m_binary[i] = (q.m_binary[i] >> k) | current;
			current = next;
		}
		if (negative){
			q.m_binary[3] |= 0xffffffff << (32 - k);
		}
	}
	return q;
}

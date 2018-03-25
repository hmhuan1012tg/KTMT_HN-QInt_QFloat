#include "StrFloat.h"


void twoComplement(string &a) // convert a binay from non-sign form to two's complement form
{
	int i;
	// reverse bit
	for (i = 0; i < a.length(); i++)
		a[i] = (a[i] - '0' + 1) % 2 + '0'; 

	// plus 1
	int j = 1;
	for (i = a.length() - 1; i >= 0; i--)
		if (j == 1 && a[i] == '1')
		{
			a[i] = '0';
			j = 1;
		}
		else {
			a[i] = (a[i] - '0' + j) + '0';
			j = 0;
		}
}

void balanceLength(string &a, string &b)
{
	int i;
	//before dot
	int bfa = 0, bfb = 0;
	for (i = 0; i < a.length() && a[i] != '.'; i++)
		bfa++;
	for (i = 0; i < b.length() && b[i] != '.'; i++)
		bfb++;
	string tmp = "0";

	//tmp.push_back(a[0]);
	for (bfa = bfa; bfa < bfb; bfa++) a.insert(0, tmp);
	//tmp.clear(); tmp.push_back(b[0]);
	for (bfb = bfb; bfb < bfa; bfb++) b.insert(0, tmp);

	//after dot
	bfa = bfb = 0;
	for (i = a.length() - 1; i >= 0 && a[i] != '.'; i--)
		bfa++;
	for (i = b.length() - 1; i >= 0 && b[i] != '.'; i--)
		bfb++;
	for (bfa = bfa; bfa < bfb && bfa <=AFTER_DOT; bfa++) a.push_back('0');
	for (bfb = bfb; bfb < bfa &&  bfb <=AFTER_DOT; bfb++) b.push_back('0');
}
//-------Class--------------
StrFloat::StrFloat()
{
	m_float = "";
	m_negative = false;
}

StrFloat::StrFloat(string p)
{
	m_float = p;
	normalize();
}

StrFloat::~StrFloat()
{
}

bool StrFloat::isNegative()
{
	return (m_negative);
}

void StrFloat::normalize() // If number is negative, eliminate sign, m_negative=true. Normalize the number 0
{
	//Check negative
	if (m_float[0] == '-') {
		m_negative = true;
		m_float.erase(0, 1);
	}
	else m_negative = false;
	
	//Keep the number after dot equal to AFTER_DOT
	int i=m_float.length()-1;
	int afd = 0;
	//Count how many numbers after dot -> afd
	while (i > -1 && m_float[i] != '.') { 
		i--;
		afd++;
	}
	if (i == -1) { //The number doesn't have . and after dot, so adding '0' to fill this.
		m_float.push_back('.');
		for (int j = 0; j <= AFTER_DOT; j++) m_float.push_back('0');
	}
	else
		if (afd < AFTER_DOT) //Add '0' to fill it
			for (int j = afd + 1; j <= AFTER_DOT; j++) m_float.push_back('0');
		else 
			while (afd > AFTER_DOT) { //Eliminate the numbers afer AFTER_DOT
				afd--;
				m_float.erase(m_float.length() - 1, 1);
			}

	//Erase the number 0 at header until it is !'0' or a '0'.
	while (m_float[0] == '0' && m_float.length() > 3) m_float.erase(0, 1); 

	//Check the situation "-0"
	for (i = 0; i < m_float.length(); i++)
		if (m_float[i] != '0' && m_float[i]!='.') return;
	m_negative = false;
}

StrFloat & StrFloat::operator+(const StrFloat & p) //Only do with two positive numbers
{
	StrFloat tmp;
	string a = m_float, b = p.m_float;
	//if (m_negative) twoComplement(a);
	//if (p.m_negative) twoComplement(b);
	balanceLength(a, b); //balance the length of number after and before dot between a and b;
	int i,j = 0;
	for (i = a.length() - 1; i >= 0; i--)
	if (a[i]!='.')
	{
		int sp = a[i] + b[i] - '0' - '0' + j;
		if (sp > 9) j = 1;
		else j = 0;
		sp %= 10;
		tmp.m_float.push_back(sp + '0');
	}
	else tmp.m_float.push_back('.');

	tmp.m_float.reserve(tmp.m_float.length()); //Reverse the arrangement of the number 
	//Need or not checking sign of result//
	tmp.normalize();
	return tmp;
}

StrFloat & StrFloat::operator=(const StrFloat & p)
{
	m_float = p.m_float;
	m_negative = p.m_negative;
}

StrFloat & StrFloat::operator/(int p) 
{
	if (p == 0) return (*this);
	if (p < 0) {
		m_negative = !m_negative;
		p = -p;
	}
	bool haveDot = false;
	string tmp = "";

	int j = 0,afd=0;
	for (int k = 0; k < m_float.length(); k++)
		if (m_float[k] == '.')
		{
			haveDot = true;
			if (tmp == "") tmp.push_back('0');
			tmp.push_back('.');
		}
		else 
		{
			j = j * 10 + m_float[k] - '0';
			if (j / p > 0) tmp.push_back((j / p) + '0');
			else
				if (haveDot) 
					tmp.push_back('0');
			j %= p;
			afd += haveDot;
		}

	if (j > 0 && !haveDot)
	{
		if (tmp.length() == 0) tmp.push_back('0');
		tmp.push_back('.');
	}
	while (j>0 && afd<AFTER_DOT) {
		j *= 10;
		tmp.push_back(j / p + '0');
		j %= p;
		afd++;
	}
	m_float = tmp;
	this->normalize();
	return (*this);
}

StrFloat & StrFloat::operator*(int p)
{
	return StrFloat();
}

StrFloat StrFloat::operator/=(int p)
{
	(*this) = (*this) / p;
	return (*this);
}

StrFloat StrFloat::operator+=(const StrFloat & p)
{
	(*this) = (*this) + p;
	return (*this);
}

StrFloat StrFloat::operator++(int x)
{
	StrFloat p("1.0");
	(*this) = (*this) + p;
	return (*this);
}

istream & operator>>(istream & is, StrFloat & p)
{
	is >> p.m_float;
	p.normalize();
	return is;
}

ostream & operator<<(ostream & os, const StrFloat & p)
{
	if (p.m_negative) os << "-";
	os << p.m_float;
	return os;
}

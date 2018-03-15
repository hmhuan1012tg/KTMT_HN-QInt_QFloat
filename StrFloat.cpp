#include "StrFloat.h"


StrFloat::StrFloat()
{
}

StrFloat::StrFloat(string p)
{
	m_float = p;
}

StrFloat::~StrFloat()
{
}

bool StrFloat::isNegative()
{
	return (m_negative);
}

StrFloat & StrFloat::operator/(int p) //only do with p>0
{
	if (p <= 0) return (*this);
	
	bool haveDot = false;
	string tmp = "";

	int j = 0;
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
				if (haveDot) tmp.push_back('0');
			j %= p;
		}
	if (j > 0 && !haveDot) tmp.push_back('0');
	while (tmp.length() + (haveDot) < 35) {
		j *= 10;
		tmp.push_back(j / p + '0');
		j %= p;
	}
	m_float = tmp;
	return (*this);
}

istream & operator>>(istream & is, StrFloat & p)
{
	is >> p.m_float;
	if (p.m_float[0] == '-'){
		p.m_negative = true;
		p.m_float.erase(0, 1);
	}
	else p.m_negative = false;

	return is;
	// TODO: insert return statement here
}

ostream & operator<<(ostream & os, const StrFloat & p)
{
	if (p.m_negative) os << "-";
	os << p.m_float;
	return os;
	// TODO: insert return statement here
}

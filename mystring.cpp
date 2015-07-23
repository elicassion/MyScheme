#include "mystring.h"
#include "boolean.h"
#include "bigint.h"
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

String::String(string str):str_(str)
{
	unitType_ = STRING;
}

String::~String()
{

}

void String::print()
{
	cout<<"\""<<str_<<"\"";
}

String* String::from_string(char* expression)
{
	int len = strlen(expression);
	if (len<2) return NULL;
	else if (expression[0]!='\"' || expression[len-1]!='\"')
		return NULL;
	else if (expression[0]=='\"' && expression[len-1]=='\"')
	{
		if (len == 3 && expression[len-2]=='\\') return NULL;
		else if (len>=4 && expression[len-2]=='\\')
		{
			if (expression[len-3]!='\\') return NULL;
		}
	}
	string s = "";
	for (int i=1;i<len-1;++i)
	{
		if (expression[i]!='\\') s += expression[i];
		else s += expression[++i];
	}
	return new String(s);
}

SchemeUnit* String::nott()
{
	return new Boolean(false);
}

SchemeUnit* String::isInteger()
{
	return new Boolean(false);
}

SchemeUnit* String::isRational()
{
	return new Boolean(false);
}

SchemeUnit* String::isReal()
{
	return new Boolean(false);
}

SchemeUnit* String::isComplex()
{
	return new Boolean(false);
}

SchemeUnit* String::isNumber()
{
	return new Boolean(false);
}

SchemeUnit* String::isChar()
{
	return new Boolean(false);
}

SchemeUnit* String::isString()
{
	return new Boolean(true);
}

String* String::makeString(Number* number, Character* c)
{
	assert(number->type_==1 
			&& "make-string parameter is non-negative integer");
	Rational* tmp = SCAST_RATIONAL(number);
	assert(tmp->den_==ONE_ 
			&& "make-string parameter is non-negative integer");
	assert(!tmp->num_.sgn_ 
			&& "make-string parameter is non-negative integer");
	string s="";
	int i_num = (int)(double)(tmp->num_);
	for (int i=1; i <= i_num; ++i)
	{
		s+=c->ch_;
	}
	return new String(s);
}

String* String::formString(Character* c)
{
	return new String(str_+=c->ch_);
}

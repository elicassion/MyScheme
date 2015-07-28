#include "character.h"
#include "boolean.h"
#include "bigint.h"
#include "rational.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
using namespace std;
Character::Character(char ch):ch_(ch)
{
    unitType_ = CHARACTER;
}

Character::~Character()
{

}

char Character::to_upper_case(char ch)
{
	if (ch>='a' && ch<='z') return (char)(ch-32);
	else if (ch>='A' && ch<='Z') return ch;
	else assert(0 && "to_up_case is only for letter");
}

char Character::to_lower_case(char ch)
{
	if (ch>='a' && ch<='z') return ch;
	else if (ch>='A' && ch<='Z') return (char)(ch+32);
	else assert(0 && "to_down_case is only for letter");
}

Character* Character::from_string(char* expression)
{
    if (strcmp(expression,"#\\newline")==0) return new Character('\n');
    if (strcmp(expression,"#\\space")==0) return new Character(' ');
    if (strlen(expression)!=3) return NULL; //to be continued
    if (!(expression[0]=='#' && expression[1]=='\\'))
        return NULL;
    return new Character(expression[2]);
}

void Character::print()
{
    printf("#\\%c",ch_);
}

SchemeUnit* Character::nott()
{
    return new Boolean(false);
}

SchemeUnit* Character::isInteger() { return new Boolean(false); }

SchemeUnit* Character::isRational() { return new Boolean(false); }

SchemeUnit* Character::isReal() { return new Boolean(false); }

SchemeUnit* Character::isComplex() { return new Boolean(false); }

SchemeUnit* Character::isNumber() { return new Boolean(false); }

SchemeUnit* Character::isChar() { return new Boolean(true); }

SchemeUnit* Character::isString() { return new Boolean(false); }

SchemeUnit* Character::chEql(Character* ch2)
{
	return new Boolean(ch_==ch2->ch_);
}

SchemeUnit* Character::chMoInc(Character* ch2)
{
	return new Boolean(ch_<ch2->ch_);
}

SchemeUnit* Character::chNonDec(Character* ch2)
{
	return new Boolean(ch_<=ch2->ch_);
}

SchemeUnit* Character::chMoDec(Character* ch2)
{
	return new Boolean(ch_>ch2->ch_);
}

SchemeUnit* Character::chNonInc(Character* ch2)
{
	return new Boolean(ch_>=ch2->ch_);
}

SchemeUnit* Character::chCiEql(Character* ch2)
{
	if (((ch_>='a' && ch_<='z') || (ch_>='A' && ch_<='Z')) 
		&& ((ch2->ch_>='a' && ch2->ch_<='z') || (ch2->ch_>='A' && ch2->ch_<='Z')))
	{
		if (abs(ch_-ch2->ch_)==0 || abs(ch_-ch2->ch_)==32)
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else return new Boolean(ch_ == ch2->ch_);
}

SchemeUnit* Character::chCiMoInc(Character* ch2)
{
	if (((ch_>='a' && ch_<='z') || (ch_>='A' && ch_<='Z')) 
		&& ((ch2->ch_>='a' && ch2->ch_<='z') || (ch2->ch_>='A' && ch2->ch_<='Z')))
	{
		if (to_upper_case(ch_) < to_upper_case(ch2->ch_))
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else return new Boolean(ch_ < ch2->ch_);
}

SchemeUnit* Character::chCiNonDec(Character* ch2)
{
	if (((ch_>='a' && ch_<='z') || (ch_>='A' && ch_<='Z')) 
		&& ((ch2->ch_>='a' && ch2->ch_<='z') || (ch2->ch_>='A' && ch2->ch_<='Z')))
	{
		if (to_upper_case(ch_) <= to_upper_case(ch2->ch_))
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else return new Boolean(ch_ <= ch2->ch_);
}

SchemeUnit* Character::chCiMoDec(Character* ch2)
{
	if (((ch_>='a' && ch_<='z') || (ch_>='A' && ch_<='Z')) 
		&& ((ch2->ch_>='a' && ch2->ch_<='z') || (ch2->ch_>='A' && ch2->ch_<='Z')))
	{
		if (to_upper_case(ch_) > to_upper_case(ch2->ch_))
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else return new Boolean(ch_ > ch2->ch_);
}

SchemeUnit* Character::chCiNonInc(Character* ch2)
{
	if (((ch_>='a' && ch_<='z') || (ch_>='A' && ch_<='Z')) 
		&& ((ch2->ch_>='a' && ch2->ch_<='z') || (ch2->ch_>='A' && ch2->ch_<='Z')))
	{
		if (to_upper_case(ch_) >= to_upper_case(ch2->ch_))
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else return new Boolean(ch_ >= ch2->ch_);
}

SchemeUnit* Character::chIsAlpha()
{
	return new Boolean((ch_>='a' && ch_<='z') || (ch_>='A' && ch_<='Z'));
}

SchemeUnit* Character::chIsNum()
{
	return new Boolean(ch_>='0' && ch_<='9');
}

SchemeUnit* Character::chIsSpace()
{
	return new Boolean(ch_==' ');
}

SchemeUnit* Character::chIsUpCase()
{
	return new Boolean(ch_>='A' && ch_<='Z');
}

SchemeUnit* Character::chIsLowCase()
{
	return new Boolean(ch_>'a' && ch_<='z');
}

SchemeUnit* Character::chToInt()
{
	return new Rational(BigInt((int)ch_),ONE_);
}

SchemeUnit* Character::chToUp()
{
	if (ch_>='a' && ch_<='z')
		return new Character((char)(ch_-32));
	else return new Character(ch_);
}

SchemeUnit* Character::chToDown()
{
	if (ch_>='A' && ch_<='Z')
		return new Character((char)(ch_+32));
	else return new Character(ch_);
}

SchemeUnit* Character::isEql(Character* ch2)
{
	return new Boolean(ch_ == ch2->ch_);
}
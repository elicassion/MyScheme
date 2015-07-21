#ifndef number_h
#define number_h

#include "schemeunit.h"
#include "boolean.h"
#define SCAST_NUMBER(x) static_cast<Number*>(x)

class Number:public SchemeUnit {
public:
	enum{
		RATIONAL = 1,
		FLOAT = 2,
		COMPLEX = 3
	} type_;
	bool exact_;
	Number();
	virtual ~Number(){}

	virtual SchemeUnit* nott();
	virtual SchemeUnit* isNumber();

	virtual Number* convert(Number* number2) = 0;
	virtual Number* add(Number* number2)  = 0;
	virtual Number* sub(Number* number2)  = 0;
	virtual Number* mul(Number* number2)  = 0;
	virtual Number* div(Number* number2)  = 0;
	virtual Number* abss() = 0;
	virtual Number* quo(Number* number2) = 0;
	virtual Number* rem(Number* number2) = 0;
	virtual Number* mod(Number* number2) = 0;
	virtual Number* gcd(Number* number2) = 0;
	virtual Number* lcm(Number* number2) = 0;
	virtual Number* expp(Number* number2) = 0;
	virtual Number* sqt() = 0;
	virtual Number* flr() = 0;
	virtual Number* cel() = 0;
	virtual Number* trc() = 0;
	virtual Number* rnd() = 0;
	virtual Number* maxi(Number* number2) = 0;
	virtual Number* mini(Number* number2) = 0;
	virtual Number* numpart() =0;
	virtual Number* denpart() =0;
	virtual Number* rpart() =0;
	virtual Number* ipart() =0;
	virtual SchemeUnit* isExact() =0;
	virtual SchemeUnit* isInexact() =0;
	virtual Number* exttoinext() =0;
	virtual Number* inexttoext() =0;
	virtual Number* sinn() =0;
	virtual Number* coss() =0;
	virtual Number* tann() =0;
	virtual Number* asinn() =0;
	virtual Number* acoss() =0;
	virtual Number* atann() =0;
	virtual SchemeUnit* eql(Number* number2) =0;
	virtual SchemeUnit* monoinc(Number* number2) =0;
	virtual SchemeUnit* mononondec(Number* number2) =0;
	virtual SchemeUnit* monodec(Number* number2) =0;
	virtual SchemeUnit* monononinc(Number* number2) =0;
	virtual SchemeUnit* isZero() =0;
	virtual SchemeUnit* isNegative() =0;
	virtual SchemeUnit* isPositive() =0;
	virtual SchemeUnit* isOdd() =0;
	virtual SchemeUnit* isEven() =0;
	virtual SchemeUnit* isInteger() =0;
    virtual SchemeUnit* isRational() =0;
    virtual SchemeUnit* isReal() =0;
    virtual SchemeUnit* isComplex() =0;
	virtual void print() = 0;
};


#endif // number_h

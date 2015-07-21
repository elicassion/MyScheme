#ifndef complex_h
#define complex_h

#include "number.h"
#include "rational.h"
#include "float.h"
#include "boolean.h"
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)

class Complex : public Number{
public:

    Number* real_;
    Number* imag_;
    //bool exact_; // 1 Rational exact, 0 Float inexact

    Complex(string r_s="0", string i_s="0");
    virtual ~Complex();

    virtual Number* convert(Number *number2);
	virtual Number* add(Number *number2);
	virtual Number* sub(Number *number2);
	virtual Number* mul(Number *number2);
	virtual Number* div(Number *number2);
	virtual Number* abss();
	virtual Number* quo(Number *number2);
	virtual Number* rem(Number *number2);
	virtual Number* mod(Number *number2);
	virtual Number* gcd(Number *number2);
	virtual Number* lcm(Number *number2);
	virtual Number* expp(Number *number2);
	virtual Number* sqt();
	virtual Number* flr();
	virtual Number* cel();
	virtual Number* trc();
	virtual Number* rnd();
	virtual Number* maxi(Number *number2);
	virtual Number* mini(Number *number2);
    virtual Number* numpart();
	virtual Number* denpart();
	virtual Number* rpart();
	virtual Number* ipart();
	virtual SchemeUnit* isExact();
	virtual SchemeUnit* isInexact();
	virtual Number* exttoinext();
	virtual Number* inexttoext();
	virtual Number* sinn();
	virtual Number* coss();
	virtual Number* tann();
	virtual Number* asinn();
	virtual Number* acoss();
	virtual Number* atann();
	virtual SchemeUnit* eql(Number* number2);
	virtual SchemeUnit* monoinc(Number* number2);
	virtual SchemeUnit* mononondec(Number* number2);
	virtual SchemeUnit* monodec(Number* number2);
	virtual SchemeUnit* monononinc(Number* number2);
	virtual SchemeUnit* isZero();
	virtual SchemeUnit* isNegative();
	virtual SchemeUnit* isPositive();
	virtual SchemeUnit* isOdd();
	virtual SchemeUnit* isEven();
	virtual SchemeUnit* isInteger();
    virtual SchemeUnit* isRational();
    virtual SchemeUnit* isReal();
    virtual SchemeUnit* isComplex();
	virtual void print();
	static Complex* from_string(char *expression);
};
#endif // complex_h

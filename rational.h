#ifndef rational_h
#define rational_h


#include "number.h"
#include "bigint.h"
#include "float.h"
#include "boolean.h"

#define SCAST_RATIONAL(x) static_cast<Rational*>(x)

static const BigInt ZERO_("0");
static const BigInt ONE_("1");
static const BigInt TWO_("2");
static const BigInt BASE_("4503599627370496");
class Rational:public Number{
public:
    BigInt num_;
	BigInt den_;

	Rational(BigInt num=ZERO_, BigInt den=ONE_);
	virtual ~Rational();

	void reduce();
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
	virtual Boolean* isexact();
	virtual Number* exttoinext();
	virtual Number* inexttoext();
	virtual Number* sinn();
	virtual Number* coss();
	virtual Number* tann();
	virtual Number* asinn();
	virtual Number* acoss();
	virtual Number* atann();
	virtual void print();
	static Rational *from_string(char *expression);
	operator double ();


};


#endif // rational_h




#ifndef float_h
#define float_h


#include "number.h"
#include <cstring>
#include <string>

#define SCAST_FLOAT(x) static_cast<Float*>(x)

class Float : public Number {
public:
	double number_;

	Float(double number = 0);
	virtual ~Float();

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
	virtual void print();
	static Float *from_string(char *expression);

};

#endif // float_h

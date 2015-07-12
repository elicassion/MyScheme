#ifndef rational_h
#define rational_h


#include "number.h"
#include "bigint.h"

#define SCAST_RATIONAL(x) static_cast<Rational*>(x)

class Rational:public Number{
public:
    BigInt num_;
	BigInt den_;

	Rational(BigInt num=0, BigInt den=1);
	virtual ~Rational();

	void reduce();
	virtual Number *convert(Number *number2);
	virtual Number *add(Number *number2);
	virtual Number *sub(Number *number2);
	virtual Number *mul(Number *number2);
	virtual Number *div(Number *number2);
	virtual void print();
	static Rational *from_string(char *expression);
	operator double ();


};


#endif // rational_h




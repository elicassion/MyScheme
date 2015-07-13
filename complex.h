#ifndef complex_h
#define complex_h

#include "number.h"
#include "rational.h"
#include "float.h"
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)

class Complex : public Number{
public:

    Number* real_;
    Number* imag_;
    //bool exact_; // 1 Rational exact, 0 Float inexact

    Complex(Number* real, Number* imag);
    virtual ~Complex();

    virtual Number* convert(Number *number2);
	virtual Number* add(Number *number2);
	virtual Number* sub(Number *number2);
	virtual Number* mul(Number *number2);
	virtual Number* div(Number *number2);

	virtual void print();
	static Complex* from_string(char *expression);
};
#endif // complex_h

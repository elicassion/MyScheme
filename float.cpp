#include "float.h"
#include "rational.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#define ABS(x) ((x)<0?(-(x)):(x))

Float::Float(double number) : number_(number)
{
	type_ = FLOAT;
}

Float::~Float()
{

}

Number *Float::convert(Number *number2)
{
	assert(number2->type_ <= type_);
	Float *result = new Float();
	switch(number2->type_)
	{
		case RATIONAL:
        {
			Rational *tmp_r = SCAST_RATIONAL(number2);
			int mul_count=0;
			double f_base = 1;
			BigInt quo = tmp_r->num_ / tmp_r->den_;
			bool MINUS = (quo<=0);
			BigInt a_quo = quo.abs();
			BigInt mul_num_ = tmp_r->num_.abs();
			BigInt a_den_ = tmp_r->den_.abs();
			while (a_quo == 0 || a_quo.s.size()<2)
            {
                mul_num_ *= BigInt::BASE;
                a_quo = mul_num_ / a_den_;
                ++mul_count;
                f_base /= BigInt::BASE;
                cout<<a_quo<<' '<<f_base<<endl;
            }
			double tmp_f = 0;
			int aq_size = a_quo.s.size();
			for (int i=aq_size-1;i>=0;--i)
            {
                tmp_f += (double)a_quo.s[i] * f_base;
                if (i)
                    tmp_f = tmp_f * BigInt::BASE;
                cout<<tmp_f<<' '<<(double)a_quo.s[i] * f_base<<endl;
            }
            if (!MINUS) result->number_=tmp_f;
            else result->number_=-tmp_f;
			break;
		}
		case FLOAT:
        {
			Float *tmp = SCAST_FLOAT(number2);
			result->number_ = tmp->number_;
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	return result;

}

Number *Float::add(Number *number2)
{
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ + tmp->number_);
	return result;
}

Number *Float::sub(Number *number2)
{
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ - tmp->number_);
	return result;
}

Number *Float::mul(Number *number2)
{
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ * tmp->number_);
	return result;
}

Number *Float::div(Number *number2)
{
	Float *tmp = SCAST_FLOAT(number2);
	assert(ABS(tmp->number_)>1e-6 && "divide zero");
	Float *result = new Float(number_ / tmp->number_);
	return result;
}

void Float::print() { cout<<number_; }

Float *Float::from_string(char *expression)
{
    char *end_pointer;
    double val = strtod(expression, &end_pointer);
    if (end_pointer == expression || end_pointer != expression + strlen(expression))
	    return NULL;
    return new Float(val);
}

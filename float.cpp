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
			BigInt quo = (tmp_r->num_ * BigInt::BASE) / tmp_r->den_;
			//cout<<quo<<endl;
			bool MINUS = (quo<=0);
			BigInt a_quo = quo.abs();
			cout<<a_quo<<endl;
			int q_size=a_quo.s.size();
			//cout<<q_size<<endl;
			double tmp_f = 0;
			for (int i=q_size-1;i>=0;--i)
            {
                tmp_f += a_quo.s[i];
                if (i!=0)
                    tmp_f *= BigInt::BASE;
            }
            //cout<<tmp_f<<endl;
            tmp_f /= BigInt::BASE;
            cout<<tmp_f<<endl;
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

void Float::print()
{
	printf("%lf\n", number_);
}

Float *Float::from_string(char *expression)
{
    char *end_pointer;
    double val = strtod(expression, &end_pointer);
    if (end_pointer == expression || end_pointer != expression + strlen(expression))
	    return NULL;
    return new Float(val);
}

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
	switch(number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->number_ = (double)tmp->numerator_ / (double)tmp->denominator_;
			break;
		}
		case FLOAT:{
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

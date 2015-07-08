#include "rational.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

Rational::Rational(BigInt num, BigInt den):num_(num),den_(den)
{
	type_=RATIONAL;
	reduce();
}
void Rational::reduce()
{
	int expression = (den_!=0) ? 1 : 0;
	assert(expression && "denominator is zero");
	if(!num_)
    {
		den_=1;
		return;
	}
	BigInt big, small, tmp;
	BigInt absnum_=num_.abs();
	BigInt absden_=den_.abs();
	big=max(absnum_, absden_);
	small=min(absnum_,absden_);
	tmp=big%small;
	while(tmp!=0)
    {
		big=small;
		small=tmp;
		tmp=big%small;
	}
	num_=num_/small;
	den_= den_/small;

	if(den_<BigInt(0))
    {
		num_ = BigInt(0)-num_;
		den_ = BigInt(0)-den_;
	}
}


Number *Rational::convert(Number *number2)
{
	assert(number2->type_ <= type_);
	Rational *result=new Rational();
	switch(number2->type_)
	{
		case RATIONAL:
        {
			Rational *tmp = SCAST_RATIONAL(number2);
			result->num_ = tmp->num_;
			result->den_ = tmp->den_;
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	result->reduce();
	return result;
}

Number *Rational::add(Number *number2)
{
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->num_ = num_*tmp->den_ + den_*tmp->num_;
	result->den_ = den_ * tmp->den_;
	result->reduce();
	return result;
}

Number *Rational::sub(Number *number2)
{
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->num_ = num_*tmp->den_ - den_*tmp->num_;
	result->den_ = den_ * tmp->den_;
	result->reduce();
	return result;
}

Number *Rational::mul(Number *number2)
{
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->num_ = num_ * tmp->num_;
	result->den_ = den_ * tmp->den_;
	result->reduce();
	return result;
}

Number *Rational::div(Number *number2)
{
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->num_ = num_ * tmp->den_;
	result->den_ = den_ * tmp->num_;
	result->reduce();
	return result;
}

void Rational::print()
{
	cout<<num_;
	if(den_ != (BigInt)1)
    {
		printf("/");
		cout<<den_;
	}
	printf("\n");
}

Rational *Rational::from_string(char *expression)
{
    char* end_pointerr;
    char *separate_pos = strchr(expression, '/');
    if(separate_pos)
    {
        BigInt num = (BigInt)strtol(expression,&end_pointerr,10);
        if( end_pointerr == expression || end_pointerr != separate_pos )
        	return NULL;
        BigInt den = (BigInt)strtol(separate_pos + 1,&end_pointerr,10);
        if( end_pointerr == separate_pos + 1 || end_pointerr != expression + strlen(expression) )
        	return NULL;
        return new Rational(num,den);
    }
    else
    {
    	BigInt num = (BigInt)strtol(expression,&end_pointerr,10);
        if( end_pointerr == expression || end_pointerr != expression + strlen(expression) )
        	return NULL;
        return new Rational(num , 1);
    }
}

// int main(){
// 	Rational *a = new Rational(-18,18);
// 	// Rational *b = new Rational(4,5);
// 	printf("%d %d\n", a->numerator_.number_, a->denominator_.number_);
// 	// (a->div(b))->print();
// }

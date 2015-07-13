#include "rational.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

Rational::Rational(BigInt num, BigInt den):num_(num),den_(den)
{
        type_=RATIONAL;
        exact_=true;
        reduce();
}

Rational::~Rational()
{

}
void Rational::reduce()
{
	//BigInt ZERO="0";
	int expression = (den_!=ZERO_) ? 1 : 0;
	assert(expression && "denominator is zero");

	if(num_==ZERO_)
    {
		den_.number_="1";
		den_.sgn_=0;
		return;
	}
	BigInt big, small, tmp;
	BigInt absnum_=num_.abs();
	BigInt absden_=den_.abs();
	big=max(absnum_, absden_);
	small=min(absnum_,absden_);
	tmp=big%small;
	//cout<<big<<' '<<small<<' '<<tmp<<endl;
	while(tmp!=ZERO_)
    {
		big=small;
		small=tmp;
		tmp=big%small;
        //cout<<big<<' '<<small<<' '<<tmp<<endl;
		//system("pause");
	}
	num_=num_/small;
	den_= den_/small;

	if(den_.sgn_)
    {
		num_=num_.minus();
		den_=den_.minus();
	}
	//cout<<num_<<' '<<den_<<endl;
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
	assert( tmp->num_!=ZERO_ && "divided by zero");
	Rational *result = new Rational();
	result->num_ = num_ * tmp->den_;
	//cout<<result->num_<<endl;
	result->den_ = den_ * tmp->num_;
	//cout<<result->den_<<endl;
	result->reduce();
	return result;
}

void Rational::print()
{
	num_.print();
	if(den_ != ONE_)
    {
		cout<<'/';
		den_.print();
	}
	//cout<<endl;
}

Rational *Rational::from_string(char *expression)
{
    char* separate_pos = strchr(expression, '/');
    char* end_pos = strchr(expression,'\0');
    char* dot_pos = strchr(expression,'.');
    char* E_pos = strchr(expression,'E');
    char* e_pos = strchr(expression,'e');
    char* i_pos = strchr(expression, 'i');
    if (i_pos) return NULL;
    if (dot_pos || E_pos || e_pos ) return NULL;
    if(separate_pos)
    {
        int num_len = separate_pos - expression;
        char* tmp_numexp_cstr = new char [num_len+2];
        strncpy(tmp_numexp_cstr, expression, num_len);
        tmp_numexp_cstr[num_len]='\0';
        string tmp_numexp=tmp_numexp_cstr;
        BigInt num = tmp_numexp;
        delete [] tmp_numexp_cstr;
        //if( end_pointerr == expression || end_pointerr != separate_pos )
        	//return NULL;
        int den_len = end_pos - separate_pos - 1;
        char* tmp_denexp_cstr = new char [den_len+2];
        strncpy(tmp_denexp_cstr, separate_pos+1, den_len);
        tmp_denexp_cstr[den_len]='\0';
        string tmp_denexp=tmp_denexp_cstr;
        BigInt den = tmp_denexp;
        delete [] tmp_denexp_cstr;
        //if( end_pointerr == separate_pos + 1 || end_pointerr != expression + strlen(expression) )
        	//return NULL;
        //Rational ttt(num,den);
        //cout<<"ttt: ";
        //ttt.print();
        return new Rational(num,den);
    }
    else
    {
    	int num_len = end_pos - expression;
    	//cout<<num_len<<endl;
        char* tmp_numexp_cstr = new char [num_len+2];
        strncpy(tmp_numexp_cstr, expression, num_len);
        tmp_numexp_cstr[num_len]='\0';
        string tmp_numexp=tmp_numexp_cstr;
        BigInt num = tmp_numexp;
        delete [] tmp_numexp_cstr;
        //Rational ttt(num,ONE_);
        //cout<<"ttt: ";
        //ttt.print();
        //if( end_pointerr == expression || end_pointerr != expression + strlen(expression) )
        	//return NULL;
        return new Rational(num,ONE_);
    }
    return NULL;
}



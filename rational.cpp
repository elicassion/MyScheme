#include "rational.h"
#include "complex.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

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

Rational* Rational::from_string(char* expression)
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

Number* Rational::abss()
{
    Rational* res = new Rational(*this);
    res->num_.sgn_= 0;
    return res;
}

Number* Rational::quo(Number *number2)
{
    //assert ( number2->type_==1 && "quotient is only for integer" );
    Rational* tmp2 = SCAST_RATIONAL(number2);
    assert ( den_==ONE_ && tmp2->den_==ONE_ && "quotient is only for integer" );
    return new Rational(num_ / tmp2->num_, ONE_);

}

Number* Rational::rem(Number *number2)
{
    assert ( number2->type_==1 && "remainder is only for integer" );
    Rational* tmp2 = SCAST_RATIONAL(number2);
    assert ( den_==ONE_ && tmp2->den_==ONE_ && "remainder is only for integer" );
    return new Rational(num_%tmp2->num_, ONE_);
}

Number* Rational::mod(Number *number2)
{
    assert ( number2->type_==1 && "modulo is only for integer" );
    Rational* tmp2 = SCAST_RATIONAL(number2);
    assert ( den_==ONE_ && tmp2->den_==ONE_ && "modulo is only for integer" );
    BigInt remi = num_ % tmp2->num_;
    if (remi.sgn_ == tmp2->num_.sgn_) return new Rational(remi,ONE_);
    else { remi = remi + tmp2->num_; return new Rational(remi,ONE_); }
}

Number* Rational::gcd(Number *number2)
{
    assert ( number2->type_==1 && "gcd is only for integer" );
    Rational* tmp2 = SCAST_RATIONAL(number2);
    assert ( den_==ONE_ && tmp2->den_==ONE_ && "gcd is only for integer" );
    if (this == number2) {return new Rational(*this); }
    BigInt big, small, tmp;
	BigInt absnum1_=num_.abs();
	BigInt absnum2_=tmp2->num_.abs();
	big=max(absnum1_, absnum2_);
	small=min(absnum1_,absnum2_);
	tmp=big%small;
	while(tmp!=ZERO_)
    {
		big=small;
		small=tmp;
		tmp=big%small;
	}
    Rational* res = new Rational(small,ONE_);
    return res;
}

Number* Rational::lcm(Number *number2)
{
    assert ( number2->type_==1 && "lcm is only for integer" );
    Rational* tmp2 = SCAST_RATIONAL(number2);
    assert ( den_==ONE_ && tmp2->den_==ONE_ && "lcm is only for integer" );
    if (this == number2) {return new Rational(*this); }
    return ((mul(tmp2))->div(gcd(tmp2)))->abss();
}

Number* Rational::expp(Number *number2)
{
    Rational* rtmp2 = SCAST_RATIONAL(number2);
    if (num_==ZERO_ && rtmp2->num_==ZERO_) return new Float(1);
    else if (num_==ZERO_ && rtmp2->num_!=ZERO_) return new Float(0);
    Float* res = new Float;
    Float* ftmp2 = SCAST_FLOAT(res->convert(number2));
    Float* ftmp1 = SCAST_FLOAT(res->convert(this));
    res->number_ = exp(ftmp2->number_ * log(ftmp1->number_));
    delete ftmp2;
    delete ftmp1;
    return res;
}

Number* Rational::sqt()
{
    Float* res = new Float;
    Float* tmp2 = SCAST_FLOAT(res->convert(this));
    if (tmp2->number_>0.0)
    {
        res->number_ = sqrt(tmp2->number_);
        delete tmp2;
        return res;
    }
    else if ((fabs(tmp2->number_)-0.0)<1e-20)
    {
        res->number_ = 0.0;
        delete tmp2;
        return res;
    }
    else
    {
        //cout<<"IMHERE "<<tmp2->number_<<endl;
        Complex* resc = new Complex;
        Float* real = new Float(0.0);
        //cout<<"sqrt: "<<sqrt(fabs(tmp2->number_))<<endl;
        Float* imag = new Float(sqrt(fabs(tmp2->number_)));
        resc->real_ = real; resc->imag_ = imag; resc->exact_=false;
        //cout<<"AA ";resc->print();
        delete res;
        delete tmp2;
        return resc;
    }
}

Number* Rational::flr()
{
    if (den_ == ONE_) return new Rational(num_, den_);
    BigInt quo = num_ / den_ ;
    if (num_.sgn_ == 0) return new Rational(quo,ONE_);
    else return new Rational(quo-ONE_,ONE_);
}

Number* Rational::cel()
{
    if (den_ == ONE_) return new Rational(num_, den_);
    BigInt quo = num_ / den_ ;
    if (num_.sgn_ == 0) return new Rational(quo+ONE_,ONE_);
    else return new Rational(quo,ONE_);
}

Number* Rational::trc()
{
    if (den_ == ONE_) return new Rational(num_, den_);
    BigInt quo = num_ / den_;
    return new Rational(quo,ONE_);
}

Number* Rational::rnd()
{
    Number* HALF_ = new Rational(ONE_,TWO_);
    if (!num_.sgn_) return add(HALF_)->flr();
    else return sub(HALF_)->cel();
}

Number* Rational::maxi(Number *number2)
{
    if (this == number2) {return new Rational(*this); }
    Rational* tmp2 = SCAST_RATIONAL(number2);
    Rational* dis = SCAST_RATIONAL(sub(tmp2));
    if (!dis->num_.sgn_) return new Rational(*this);
    else return new Rational(*tmp2);
}

Number* Rational::mini(Number *number2)
{
    if (this == number2) {return new Rational(*this); }
    Rational* tmp2 = SCAST_RATIONAL(number2);
    Rational* dis = SCAST_RATIONAL(sub(tmp2));
    if (dis->num_.sgn_) return new Rational(*this);
    else return new Rational(*tmp2);
}

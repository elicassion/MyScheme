#include "float.h"
#include "rational.h"
#include "complex.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <string>
#include <cmath>
#define ABS(x) ((x)<0?(-(x)):(x))
static const double EPS=1e-20;
using namespace std;
Float::Float(double number) : number_(number)
{
	type_ = FLOAT;
	exact_=false;
}

Float::~Float()
{

}

Number *Float::convert(Number *number2)
{
	assert(number2->type_ <= type_);
	//cout<<number2->type_<<endl;
	Float *result = new Float();
	switch(number2->type_)
	{
		case RATIONAL:
        {
			Rational *tmp_r = SCAST_RATIONAL(number2);
			//result->number_ = (double) *tmp_r;
			result->number_ = (double) tmp_r->num_ / (double)tmp_r->den_;
            //cout<<result->number_<<endl;
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
	//cout<<number_<<' '<<tmp->number_<<endl;
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
	assert(ABS(tmp->number_)>1e-300 && "divide zero");
	Float *result = new Float(number_ / tmp->number_);
	return result;
}

void Float::print()
{
    if (fabs(number_)<=1e-20 || fabs(number_)>=1e20)
        cout<<resetiosflags(ios::fixed)<<setprecision(30)<<number_;
    else
        cout<<fixed<<setprecision(30)<<number_;
}

Float *Float::from_string(char *expression)
{
    char *end_pointer;
    char* i_pos = strchr(expression, 'i');
    if (i_pos) return NULL;
    double val = 0;
    val = strtod(expression, &end_pointer);
    if (end_pointer == expression || end_pointer != expression + strlen(expression))
	    return NULL;
    //cout<<val<<endl;
    return new Float(val);
}

Number* Float::abss()
{
    return new Float(ABS(number_));
}

Number* Float::quo(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<1e-20)
        assert(0 && "divided by zero");
    bool sgn1 = (number_<0.0);
    bool sgn2 = (tmp2->number_<0.0);
    if (fabs(number_) >1e17 && fabs(tmp2->number_) > 1e17)
    {
        Float* tmp = new Float(number_/tmp2->number_);
        Number* res = tmp->trc();
        delete tmp; return res;
    }
    else if (fabs(number_)>1e17 && !(fabs(tmp2->number_)>1e17))
    {
        long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
        if (fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
            assert(0 && "quotient is only for integer");
        Float* tmp = new Float(number_/tmp2->number_);
        Number* res = tmp->trc();
        delete tmp; return res;
    }
    else if (!(fabs(number_)>1e17) && fabs(tmp2->number_)>1e17)
    {
        long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
        if (fabs(fabs(number_)-(double)a_i_num1)>EPS)
            assert(0 && "quotient is only for integer");
        return new Float(0.0);
    }
    else
    {
        long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
        long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
        if (fabs(fabs(number_)-(double)a_i_num1)>EPS ||
            fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
            assert(0 && "quotient is only for integer");
        if (!(sgn1^sgn2))   return new Float(a_i_num1/a_i_num2);
        else return new Float(0-a_i_num1/a_i_num2);
    }
}

Number* Float::rem(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<1e-20)
        assert(0 && "divided by zero");
    bool sgn1 = (number_<0.0);
    bool sgn2 = (tmp2->number_<0.0);
    double dquo = number_/tmp2->number_;
    if (fabs(number_) > 1e17 && fabs(tmp2->number_) > 1e17 && fabs(dquo)>1e17) return new Float(0.0);
    else if (fabs(number_) > 1e17 && fabs(tmp2->number_) > 1e17 && !(fabs(dquo)>1e17))
    {
        if (fabs(number_)<fabs(tmp2->number_))return new Float(number_);
        else
        {
            Float* tmp = new Float(number_-((double)((long long int)dquo))*tmp2->number_);
            Number* res = tmp->trc();
            delete tmp; return res;
        }
    }
    else if (fabs(number_)>1e17 && !(fabs(tmp2->number_)>1e17))
    {
        long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
            if (fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
                assert(0 && "remainder is only for integer");
        if (fabs(dquo)>1e17) return new Float(0.0);
        else
        {
            Float* tmp = new Float(number_-((double)((long long int)dquo))*tmp2->number_);
            Number* res = tmp->trc();
            delete tmp; return res;
        }
    }
    else if (!(fabs(number_)>1e17) && fabs(tmp2->number_)>1e17)
    {
        long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
        if (fabs(fabs(number_)-(double)a_i_num1)>EPS)
            assert(0 && "quotient is only for integer");
        return new Float(number_);
    }
    else
    {
        long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
        long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
        if (fabs(fabs(number_)-(double)a_i_num1)>EPS ||
            fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
            assert(0 && "remainder is only for integer");
        if (!sgn1)   return new Float(a_i_num1%a_i_num2);
        else return new Float(0-a_i_num1/a_i_num2);
    }
}

Number* Float::mod(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<1e-20)
        assert(0 && "divided by zero");
    bool sgn1 = (number_<0.0);
    bool sgn2 = (tmp2->number_<0.0);
    double dquo = number_/tmp2->number_;
    if (fabs(number_) > 1e17 && fabs(tmp2->number_) > 1e17 && fabs(dquo)>1e17) return new Float(0.0);
    else if (fabs(number_) > 1e17 && fabs(tmp2->number_) > 1e17 && !fabs(dquo)>1e17)
    {
        if (fabs(number_)<fabs(tmp2->number_))return new Float(number_);
        else
        {
            Float* tmp = new Float(number_-((double)((long long int)dquo))*tmp2->number_);
            Float* res = SCAST_FLOAT(tmp->trc());
            delete tmp;
            if (!sgn2) return new Float(fabs(res->number_));
            else return new Float(0.0-fabs(res->number_));
        }
    }
    else if (fabs(number_)>1e17 && !(fabs(tmp2->number_)>1e17))
    {
        long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
            if (fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
                assert(0 && "modulo is only for integer");
        if (fabs(dquo)>1e17) return new Float(0.0);
        else
        {
            Float* tmp = new Float(number_-((double)((long long int)dquo))*tmp2->number_);
            Float* res = SCAST_FLOAT(tmp->trc());
            delete tmp;
            if (!sgn2) return new Float(fabs(res->number_));
            else return new Float(0.0-fabs(res->number_));
        }
    }
    else if (!(fabs(number_)>1e17) && fabs(tmp2->number_)>1e17)
    {
        long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
        if (fabs(fabs(number_)-(double)a_i_num1)>EPS)
            assert(0 && "modulo is only for integer");
        if (!sgn2) return new Float(fabs(number_));
        else return new Float(0.0-fabs(number_));
    }
    else
    {
        long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
        long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
        if (fabs(fabs(number_)-(double)a_i_num1)>EPS ||
            fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
            assert(0 && "modulo is only for integer");
        if (!sgn2)   return new Float(a_i_num1%a_i_num2);
        else return new Float(0.0-a_i_num1/a_i_num2);
    }
}

Number* Float::gcd(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(number_)<1e-20 || fabs(tmp2->number_)<1e-20)
        assert(0 && "divided by zero");
    long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
    long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
    if (fabs(fabs(number_)-(double)a_i_num1)>EPS ||
        fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
            assert(0 && "gcd is only for integer");
    long long int big,small,tmp;
	big=max(a_i_num1, a_i_num2);
	small=min(a_i_num1,a_i_num2);
	tmp=big%small;
	while(tmp!=0)
    {
		big=small;
		small=tmp;
		tmp=big%small;
	}
    Float* res = new Float((double)small);
    return res;
}

Number* Float::lcm(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(number_)<1e-20 || fabs(tmp2->number_)<1e-20)
        assert(0 && "divided by zero");
    long long int a_i_num1 = (long long int)(fabs(number_)+0.5);
    long long int a_i_num2 = (long long int)(fabs(tmp2->number_)+0.5);
    if (fabs(fabs(number_)-(double)a_i_num1)>EPS ||
        fabs(fabs(tmp2->number_)-(double)a_i_num2)>EPS)
            assert(0 && "gcd is only for integer");
    long long int big,small,tmp;
	big=max(a_i_num1, a_i_num2);
	small=min(a_i_num1,a_i_num2);
	tmp=big%small;
	while(tmp!=0)
    {
		big=small;
		small=tmp;
		tmp=big%small;
	}
    Float* res = new Float(number_*tmp2->number_/small);
}

Number* Float::expp(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(number_)<EPS && fabs(tmp2->number_)<EPS) return new Float(1);
    else if (fabs(number_)<EPS && fabs(tmp2->number_)>EPS) return new Float(0);
    return new Float(exp(number_ * log(tmp2->number_)));
}

Number* Float::sqt()
{
    if (number_>0.0) return new Float(sqrt(number_));
    else if ((fabs(number_)-0.0)<1e-20) return new Float(0.0);
    else
    {
        Complex* resc = new Complex;
        Float* real = new Float(0.0);
        Float* imag = new Float(sqrt(fabs(number_)));
        resc->real_ = real; resc->imag_ = imag; resc->exact_=false;
        return resc;
    }
}

Number* Float::flr() {return new Float(floor(number_)); }

Number* Float::cel() {return new Float(ceil(number_)); }

Number* Float::trc() {return new Float((number_>0.0)?floor(number_):ceil(number_)); }

Number* Float::rnd() {return new Float((number_>0.0)?floor(number_+0.5):ceil(number_-0.5)); }

Number* Float::maxi(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Float((number_>tmp2->number_)?number_:tmp2->number_);
}

Number* Float::mini(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Float((number_<tmp2->number_)?number_:tmp2->number_);
}

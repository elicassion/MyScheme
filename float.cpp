#include "float.h"
#include "rational.h"
#include "complex.h"
#include "boolean.h"
#include <iostream>
#include <complex>
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
static const double EPS=1e-200;
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
	//assert(ABS(tmp->number_)>1e-300 && "divide zero");
	Float *result = new Float(number_ / tmp->number_);
	return result;
}

void Float::print()
{
    if ((fabs(number_)<=1e-10 || fabs(number_)>=1e10) && fabs(number_)>1e-300)
        cout<<resetiosflags(ios::fixed)<<setprecision(10)<<number_;
    else
        cout<<fixed<<setprecision(10)<<number_;
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
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "quotient is only for integer");
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->quo(r_tmp2));
}

Number* Float::rem(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "remainder is only for integer");
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->rem(r_tmp2));
}

Number* Float::mod(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "modulo is only for integer");
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->mod(r_tmp2));
}

Number* Float::gcd(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(number_)<1e-300)
        return new Float(fabs(tmp2->number_));
    if (fabs(tmp2->number_)<1e-300)
        return new Float(fabs(number_));
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->gcd(r_tmp2));
}

Number* Float::lcm(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<1e-300 || fabs(number_)<1e-300)
        return new Float(0.0);
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->lcm(r_tmp2));
}

Number* Float::expp(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if(number_>=0)
        return new Float(pow(number_, tmp2->number_));
    else
    {
        Complex* c = new Complex;
        c = SCAST_COMPLEX(c->convert(this));
        Complex* d = SCAST_COMPLEX(c->convert(tmp2));
        return c->expp(d);
    }
}

Number* Float::expe()
{
    return new Float(exp(number_));
}

Number* Float::logg()
{
    if (number_>0)
    {
        return new Float(log(number_));
    }
    else
    {
        complex<double> c_x(number_,0.0);
        complex<double> c_res = log(c_x);
        Complex* res = new Complex;
        res->exact_ = false;
        res->real_ = new Float(::real(c_res));
        res->imag_ = new Float(::imag(c_res));
        return res;
    }
}

Number* Float::sqt()
{
    if (number_>=0.0) return new Float(sqrt(number_));
    else
    {
        Complex* resc = new Complex;
        Float* real = new Float(0.0);
        Float* imag = new Float(sqrt(fabs(number_)));
        resc->real_ = real; resc->imag_ = imag; resc->exact_=false;
        return resc;
    }
}

Number* Float::flr() { return new Float(floor(number_)); }

Number* Float::cel() { return new Float(ceil(number_)); }

Number* Float::trc() { return new Float(trunc(number_)); }

Number* Float::rnd() { return new Float(nearbyint(number_)); }

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

Number* Float::numpart()
{
    Rational* tmp = SCAST_RATIONAL(inexttoext());
    return new Rational(tmp->num_,ONE_);
}

Number* Float::denpart()
{
    Rational* tmp = SCAST_RATIONAL(inexttoext());
    return new Rational(tmp->den_,ONE_);
}

Number* Float::rpart() {return new Float(number_); }

Number* Float::ipart() {return new Float(0.0); }

Number* Float::makeRec(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    Complex* c = new Complex;
    c->exact_ = false;
    c->real_ = new Float(number_);
    c->imag_ = new Float(tmp2->number_);
    return c;
}

Number* Float::makePol(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    complex<double> c_res = polar(number_,tmp2->number_);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    return res;
}

Number* Float::magnt()
{
    return new Float(fabs(number_));
}

Number* Float::ang()
{
    if (number_>=0.0) return new Rational(ZERO_,ONE_);
    else return new Float(3.1415926535897932384);
}

SchemeUnit* Float::isExact() { return new Boolean(false); }

SchemeUnit* Float::isInexact() { return new Boolean(true); }

Number* Float::exttoinext() { return new Float(number_); }

Number* Float::inexttoext()
{
    double c_number_ = number_;
    double bs = 1.0;
    while(fabs(c_number_-nearbyint(c_number_))>1e-300)
    {
        c_number_*=2;
        bs*=2;
    }
    stringstream ss1,ss2;
    string s1,s2;
    ss1<<fixed<<setprecision(5)<<c_number_;
    ss2<<fixed<<setprecision(5)<<bs;
    ss1>>s1; ss2>>s2;
    s1=s1.substr(0,s1.find("."));
    s2=s2.substr(0,s2.find("."));
    BigInt num(s1),den(s2);
    return new Rational(num,den);

}

Number* Float::sinn() { return new Float(sin(number_)); }

Number* Float::coss() { return new Float(cos(number_)); }

Number* Float::tann() { return new Float(tan(number_)); }

Number* Float::asinn() { return new Float(asin(number_)); }

Number* Float::acoss() { return new Float(acos(number_)); }

Number* Float::atann() { return new Float(atan(number_)); }

SchemeUnit* Float::eql(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Boolean(fabs(number_-tmp2->number_)<1e-307);
}

SchemeUnit* Float::moInc(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Boolean(tmp2->number_>number_);
}

SchemeUnit* Float::nonDec(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Boolean(tmp2->number_>=number_);
}

SchemeUnit* Float::moDec(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Boolean(tmp2->number_<number_);
}

SchemeUnit* Float::nonInc(Number* number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    return new Boolean(tmp2->number_<=number_);
}

SchemeUnit* Float::isZero()
{
    return new Boolean(fabs(number_)<1e-307);
}

SchemeUnit* Float::isNegative()
{
    return new Boolean(number_<0);
}

SchemeUnit* Float::isPositive()
{
    return new Boolean(number_>0);
}

SchemeUnit* Float::isOdd()
{
    return isEven()->nott();
}

SchemeUnit* Float::isEven()
{
    assert(fabs(number_-nearbyint(number_))<1e-300 && "even? is only for integer");
    double half_number_ = number_ /2.0;
    return new Boolean(fabs(half_number_-nearbyint(half_number_))<1e-300);
}

SchemeUnit* Float::isInteger()
{
    return new Boolean(fabs(number_-nearbyint(number_))<1e-300);
}

SchemeUnit* Float::isRational()
{
    return new Boolean(true);
}

SchemeUnit* Float::isReal()
{
    return new Boolean(true);
}

SchemeUnit* Float::isComplex()
{
    return new Boolean(false);
}

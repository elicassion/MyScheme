#include "float.h"
#include "rational.h"
#include "complex.h"
#include "boolean.h"
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
	assert(ABS(tmp->number_)>1e-300 && "divide zero");
	Float *result = new Float(number_ / tmp->number_);
	return result;
}

void Float::print()
{
    if ((fabs(number_)<=1e-20 || fabs(number_)>=1e20) && fabs(number_)>1e-300)
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
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "quotient is only for integer");
    stringstream ss1;
    ss1<<fixed<<setprecision(5)<<number_;
    string s1;
    ss1>>s1;
    int dot_pos_1 = s1.find(".");
    string num1_s = s1.substr(0,dot_pos_1);
    BigInt num1(num1_s);
    stringstream ss2;
    ss2<<fixed<<setprecision(5)<<tmp2->number_;
    string s2;
    ss2>>s2;
    int dot_pos_2 = s2.find(".");
    string num2_s = s2.substr(0,dot_pos_2);
    BigInt num2(num2_s);
    Rational* r_quo = new Rational(num1/num2,ONE_);
    Number* res = convert(r_quo);
    delete r_quo;
    return res;
}

Number* Float::rem(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<EPS)
        assert(0 && "divided by zero");
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "quotient is only for integer");
    stringstream ss1;
    ss1<<fixed<<setprecision(5)<<number_;
    string s1;
    ss1>>s1;
    int dot_pos_1 = s1.find(".");
    string num1_s = s1.substr(0,dot_pos_1);
    BigInt num1(num1_s);
    stringstream ss2;
    ss2<<fixed<<setprecision(5)<<tmp2->number_;
    string s2;
    ss2>>s2;
    int dot_pos_2 = s2.find(".");
    string num2_s = s2.substr(0,dot_pos_2);
    BigInt num2(num2_s);
    Rational* r_rem = new Rational(num1%num2,ONE_);
    Number* res = convert(r_rem);
    delete r_rem;
    return res;
}

Number* Float::mod(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<EPS)
        assert(0 && "divided by zero");
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "quotient is only for integer");
    bool sgn2 = (tmp2->number_<0);
    stringstream ss1;
    ss1<<fixed<<setprecision(5)<<number_;
    string s1;
    ss1>>s1;
    int dot_pos_1 = s1.find(".");
    string num1_s = s1.substr(0,dot_pos_1);
    BigInt num1(num1_s);
    stringstream ss2;
    ss2<<fixed<<setprecision(5)<<tmp2->number_;
    string s2;
    ss2>>s2;
    int dot_pos_2 = s2.find(".");
    string num2_s = s2.substr(0,dot_pos_2);
    BigInt num2(num2_s);
    BigInt i_rem = num1%num2;
    Rational* r_rem;
    if (!(sgn2^i_rem.sgn_)) r_rem = new Rational(i_rem,ONE_);
    else { i_rem = i_rem + num2; r_rem = new Rational(i_rem,ONE_); }
    Number* res = convert(r_rem);
    delete r_rem;
    return res;
}

Number* Float::gcd(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<EPS)
        assert(0 && "divided by zero");
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "quotient is only for integer");
    bool sgn2 = (tmp2->number_<0);
    stringstream ss1;
    ss1<<fixed<<setprecision(5)<<number_;
    string s1;
    ss1>>s1;
    int dot_pos_1 = s1.find(".");
    string num1_s = s1.substr(0,dot_pos_1);
    BigInt num1(num1_s);
    stringstream ss2;
    ss2<<fixed<<setprecision(5)<<tmp2->number_;
    string s2;
    ss2>>s2;
    int dot_pos_2 = s2.find(".");
    string num2_s = s2.substr(0,dot_pos_2);
    BigInt num2(num2_s);
    BigInt big,small,tmp;
	big=max(num1, num2);
	small=min(num1,num2);
	tmp=big%small;
	while(tmp!=ZERO_)
    {
		big=small;
		small=tmp;
		tmp=big%small;
	}
    Float* res = new Float((double)small.abs());
    return res;
}

Number* Float::lcm(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<EPS)
        assert(0 && "divided by zero");
    assert((number_==nearbyint(number_) && tmp2->number_ == nearbyint(tmp2->number_))
            && "quotient is only for integer");
    bool sgn2 = (tmp2->number_<0);
    stringstream ss1;
    ss1<<fixed<<setprecision(5)<<number_;
    string s1;
    ss1>>s1;
    int dot_pos_1 = s1.find(".");
    string num1_s = s1.substr(0,dot_pos_1);
    BigInt num1(num1_s);
    stringstream ss2;
    ss2<<fixed<<setprecision(5)<<tmp2->number_;
    string s2;
    ss2>>s2;
    int dot_pos_2 = s2.find(".");
    string num2_s = s2.substr(0,dot_pos_2);
    BigInt num2(num2_s);
    BigInt big,small,tmp;
	big=max(num1, num2);
	small=min(num1,num2);
	tmp=big%small;
	while(tmp!=ZERO_)
    {
		big=small;
		small=tmp;
		tmp=big%small;
	}
    Float* res = new Float((double)((num1*num2/small).abs()));
    return res;
}

Number* Float::expp(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(number_)<EPS && fabs(tmp2->number_)<EPS) return new Float(1);
    else if (fabs(number_)<EPS && fabs(tmp2->number_)>EPS) return new Float(0);
    return new Float(exp(tmp2->number_ * log(number_)));
}

Number* Float::sqt()
{
    if (number_>0.0) return new Float(sqrt(number_));
    else if (fabs(number_)<EPS) return new Float(0.0);
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

Number* Float::numpart() { return NULL; }

Number* Float::denpart() { return NULL; }

Number* Float::rpart() {return new Float(number_); }

Number* Float::ipart() {return new Float(0.0); }

Boolean* Float::isexact() { return new Boolean(false);}

Number* Float::exttoinext() { return new Float(number_); }

Number* Float::inexttoext()
{
    if (fabs(number_)<=1e-6) return new Rational(ZERO_,ONE_);
    else
    {
        stringstream ss;
        //if (fabs(number_)<1e10) ss<<number_;
        /*else*/ ss<<fixed<<setprecision(5)<<number_;
        string s;
        ss>>s;
        int dot_pos = s.find(".");
        BigInt num(s.substr(0,dot_pos)+s.substr(dot_pos+1,s.length()-dot_pos-1));
        string den_s="1";
        for (int i=1;i<s.length()-dot_pos;++i)
            den_s+="0";
        BigInt den(den_s);
        Rational* res = new Rational(num,den);
        return res;
    }
}

Number* Float::sinn() { return new Float(sin(number_)); }

Number* Float::coss() { return new Float(cos(number_)); }

Number* Float::tann() { return new Float(tan(number_)); }

Number* Float::asinn() { return new Float(asin(number_)); }

Number* Float::acoss() { return new Float(acos(number_)); }

Number* Float::atann() { return new Float(atan(number_)); }

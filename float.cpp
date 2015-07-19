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
	//assert(ABS(tmp->number_)>1e-300 && "divide zero");
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
        return new Float(tmp2->number_);
    if (fabs(tmp2->number_)<1e-300)
        return new Float(number_);
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->lcm(r_tmp2));
}

Number* Float::lcm(Number *number2)
{
    Float* tmp2 = SCAST_FLOAT(number2);
    if (fabs(tmp2->number_)<1e-300 || fabs(number_)<1e-300)
        return new Float(0.0);
    Rational* r_tmp2 = SCAST_RATIONAL(tmp2->inexttoext());
    Rational* r_tmp1 = SCAST_RATIONAL(inexttoext());
    return convert(r_tmp1->rem(r_tmp2));
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

Boolean* Float::isexact() { return new Boolean(false);}

Number* Float::exttoinext() { return new Float(number_); }

Number* Float::inexttoext()
{
    if (fabs(nearbyint(number_)-number_)<1e-300)
    {
        stringstream ss1;
        ss1<<fixed<<setprecision(5)<<number_;
        string s1;
        ss1>>s1;
        int dot_pos_1 = s1.find(".");
        string num1_s = s1.substr(0,dot_pos_1);
        BigInt num1(num1_s);
        return new Rational(num1,ONE_);
    }
    double *px = &number_;
    long long int* py=reinterpret_cast<long long int*>(px);
    long long int y=*py;
    long long int p=1;
    bool v[100]={0};
    for (int i=1;i<=64;++i) { v[i] = y&p; y>>=1; }
    int tmp=0;
    for (int i=63;i>=53;--i) { tmp<<=1; tmp+=v[i]; }
    tmp-=1022;
    for (int i=1;i<=11;++i) { v[52+i] = tmp&p; tmp>>=1; }
    long long int i_num=0;
    for (int i=63;i>=1;--i) { i_num<<=1; i_num+=v[i]; }
    string s_num="";
    while (i_num!=0)
    {
        s_num+=(char)(i_num%10+'0');
        i_num/=10;
    }
    std::reverse(s_num.begin(),s_num.end());
    BigInt num = s_num;
    return new Rational(num,BASE_);
}

Number* Float::sinn() { return new Float(sin(number_)); }

Number* Float::coss() { return new Float(cos(number_)); }

Number* Float::tann() { return new Float(tan(number_)); }

Number* Float::asinn() { return new Float(asin(number_)); }

Number* Float::acoss() { return new Float(acos(number_)); }

Number* Float::atann() { return new Float(atan(number_)); }

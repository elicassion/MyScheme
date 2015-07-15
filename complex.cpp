#include "complex.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#define ABS(x) ((x)<0?(-(x)):(x))
Complex::Complex(string r_s, string i_s):real_(NULL) , imag_(NULL)
{
    //cout<<"real s: "<<r_s<<" imag s:"<<i_s<<endl;
    type_=COMPLEX;
    Number* real = NULL;
    Number* imag = NULL;
    bool isExact=1;
    bool r_isExact=1, i_isExact=1;
    //convert string to char*
    int r_s_l = r_s.length(), i_s_l = i_s.length();
    char* crs = new char [r_s_l+2];
    char* cis = new char [i_s_l+2];
    for (int i=0; i<r_s_l; ++i) crs[i]=r_s[i];
    crs[r_s_l]='\0';
    for (int i=0; i<i_s_l; ++i) cis[i]=i_s[i];
    cis[i_s_l]='\0';

    //get pointer
    real = Rational::from_string(crs);
    if (!real) { real = Float::from_string(crs); if (real) { isExact=0; r_isExact=0;} }
    if (!real) assert(0 && "input error no such type");

    imag = Rational::from_string(cis);
    if (!imag) { imag = Float::from_string(cis); if (imag) { isExact=0; i_isExact=0;} }
    if (!imag) assert(0 && "input error no such type");

    //check exact
    if (isExact) {real_ = real; imag_ = imag; exact_ = true; }
    else
    {
        exact_ = false;
        if (r_isExact && !i_isExact)
        {
            Float* tmp2 = SCAST_FLOAT(imag);
            Number* pre_r = real;
            real = tmp2->convert(real);
            delete pre_r;
            real_ = real;
            imag_ = imag;
        }
        else if (!r_isExact && i_isExact)
        {
            Float* tmp1 = SCAST_FLOAT(real);
            Number* pre_i = imag;
            imag = tmp1->convert(imag);
            delete pre_i;
            real_ = real;
            imag_ = imag;
        }
        else {real_ = real; imag_ = imag; exact_ = false; }
    }

}

Complex::~Complex()
{
    delete real_;
    delete imag_;
}

Number* Complex::convert(Number* number2)
{
	assert(number2->type_ <= type_);
	Complex* result=new Complex();
	//cout<<"before result exact: "<<result->exact_<<endl;
	//cout<<"number2 type: "<<number2->type_<<endl;
	switch(number2->type_)
	{
		case RATIONAL:
        {
			if (exact_)
            {
                result->exact_ = true;
                Rational* tmp1 = new Rational;
                *tmp1 = *SCAST_RATIONAL(number2);
                result->real_ = tmp1;
                Rational* tmp2 = new Rational;
                result->imag_ = tmp2;
                //cout<<"after result exact: "<<result->exact_<<endl;
                break;
            }
            else
            {
                //cout<<"FUCK"<<endl;
                result->exact_ = false;
                Float* tmp1 = new Float;
                result->real_ = tmp1->convert(number2);
                delete tmp1;
                //result->real_->print();

                Float* tmp2 = new Float;
                result->imag_ = tmp2;

                //result->imag_->print();

                //cout<<"after result exact: "<<result->exact_<<endl;
                break;
            }

		}
		case FLOAT:
        {
            result->exact_ = number2->exact_;
            Float* tmp1 = new Float;
            *tmp1 = *SCAST_FLOAT(number2);
            result->real_ = tmp1;
            Float* tmp2 = new Float;
            result->imag_ = tmp2;

            if (exact_)
            {
                exact_ = false;
                Number *pre_real_ =real_, *pre_imag_=imag_;
                real_ = tmp2->convert(real_);
                imag_ = tmp2->convert(imag_);
                delete pre_real_; delete pre_imag_;
            }
            break;

        }
        case COMPLEX:
        {
            Complex* tmp = SCAST_COMPLEX(number2);
            if (exact_ && tmp->exact_)
            {
                Rational* tmp1 = new Rational;
                *tmp1 = *SCAST_RATIONAL(tmp->real_);
                result->real_ = tmp1;

                Rational* tmp2 = new Rational;
                *tmp2 = *SCAST_RATIONAL(tmp->imag_);
                result->imag_ = tmp2;

                result->exact_ = tmp->exact_;
            }
            else if (!exact_ && !tmp->exact_)
            {
                Float* tmp1 = new Float;
                *tmp1 = *SCAST_FLOAT(tmp->real_);
                result->real_ = tmp1;

                Float* tmp2 = new Float;
                *tmp2 = *SCAST_FLOAT(tmp->imag_);
                result->imag_ = tmp2;

                result->exact_ = tmp->exact_;

                /*cout<<endl;
                cout<<"in convert 0 0 : ";
                result->print();
                cout<<endl;*/
            }
            else if (!exact_ && tmp->exact_)
            {
                Float* tmp1 = new Float;
                *tmp1 = *SCAST_FLOAT(tmp1->convert(tmp->real_));
                result->real_ = tmp1;

                Float* tmp2 = new Float;
                *tmp2 = *SCAST_FLOAT(tmp1->convert(tmp->imag_));
                result->imag_ = tmp2;

                result->exact_ = exact_;
            }
            else assert(0 && "inexact number convert to exact number");
            break;
        }
		default:
			assert(0 && "type_ not defined");
	}
	/*cout<<"in convert final: ";
	result->print();
	cout<<endl;*/
	return result;
}

Number* Complex::add(Number *number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	result->real_ = real_->add(tmp2->real_);
	result->imag_ = imag_->add(tmp2->imag_);
	result->exact_ = exact_;
	return result;
}

Number* Complex::sub(Number* number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	result->real_ = real_->sub(tmp2->real_);
	result->imag_ = imag_->sub(tmp2->imag_);
	result->exact_ = exact_;
	return result;
}

Number* Complex::mul(Number* number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	result->real_ = (real_->mul(tmp2->real_))->sub(imag_->mul(tmp2->imag_));
    result->imag_ = (real_->mul(tmp2->imag_))->add(imag_->mul(tmp2->real_));
    result->exact_ = exact_;
	return result;
}

Number* Complex::div(Number* number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	Number* sqrsum;
	sqrsum = (tmp2->real_->mul(tmp2->real_))->add(tmp2->imag_->mul(tmp2->imag_));
	result->real_ = ((real_->mul(tmp2->real_))->add(imag_->mul(tmp2->imag_)))->div(sqrsum);
	result->imag_ = ((imag_->mul(tmp2->real_))->sub(real_->mul(tmp2->imag_)))->div(sqrsum);
	result->exact_ = exact_;
	return result;
}


Complex* Complex::from_string(char* expression)
{
    string str = expression;
    string real_str, imag_str;
    int len = str.length();
    if (str[len-1]!='i') return NULL;
    if (len==1 && str[0]=='i') { real_str = "0"; imag_str = "1"; }
    else if (len==2 && str[0]=='-' && str[1]=='i') { real_str = "0"; imag_str = "-1";}
    else
    {
        int imag_part_begin = str.length()-1;
        for (; imag_part_begin>=0; --imag_part_begin)
            if (str[imag_part_begin] == '+' || str[imag_part_begin] == '-')
                if (imag_part_begin==0 || (str[imag_part_begin-1]!='e' && str[imag_part_begin-1]!='E'))
                    break;
        if (imag_part_begin==-1) imag_part_begin=0;
        real_str = (imag_part_begin==0) ? "0" : str.substr(0,imag_part_begin);
        imag_str = str.substr(imag_part_begin, len - imag_part_begin-1);
        //cout<<real_str<<' '<<imag_str<<endl;
        if (imag_str == "+") imag_str="1";
        else if (imag_str == "-") imag_str="-1";
    }
    return new Complex(real_str, imag_str);
}

void Complex::print()
{
	bool real_no_zero=1;
	if (exact_)
    {
        Rational* tmp_real_ = SCAST_RATIONAL(real_);
        if (BigInt::abs_cmp(tmp_real_->num_.number_,"0")==0)
            real_no_zero=0;
        if (real_no_zero) tmp_real_->print();

        Rational* tmp_imag_ = SCAST_RATIONAL(imag_);
        int FLAG=BigInt::abs_cmp(tmp_imag_->num_.number_,"0");
        //tmp_imag_->num_.print();
        //cout<<endl;
        if (FLAG==1 && real_no_zero && !tmp_imag_->num_.sgn_) printf("+");
        else if (FLAG==0 && !real_no_zero) { cout<<'0'; return; }
        else if (FLAG==0 && real_no_zero) return;
        if (tmp_imag_->num_.number_ == "1" && tmp_imag_->den_.number_ == "1" )
        {
            //cout<<"LLLL"<<endl;
            if (!tmp_imag_->num_.sgn_) { printf("i"); return; }
            else {printf("-i"); return; }
        }
        tmp_imag_->print();
    }

    else
    {
        Float* tmp_real_ = SCAST_FLOAT(real_);
        if (ABS(tmp_real_->number_)<1e-300)
            real_no_zero=0;
        if (real_no_zero) tmp_real_->print();

        Float* tmp_imag_ = SCAST_FLOAT(imag_);
        if (tmp_imag_->number_>0 && real_no_zero)
            printf("+");
        else if (ABS(tmp_imag_->number_)<1e-300 && !real_no_zero) { double x=0.0; cout<<x; return; }
        else if (ABS(tmp_imag_->number_)<1e-300 && real_no_zero) return;
        tmp_imag_->print();
    }
    //cout<<"FUCK"<<endl;
	printf("i");
}

Number* Complex::abss() {return NULL; }
Number* Complex::quo(Number *number2) {return NULL; }
Number* Complex::rem(Number *number2) {return NULL; }
Number* Complex::mod(Number *number2) {return NULL; }
Number* Complex::gcd(Number *number2) {return NULL; }
Number* Complex::lcm(Number *number2) {return NULL; }
Number* Complex::expp(Number *number2) {return NULL; }
Number* Complex::sqt() {return NULL; }
Number* Complex::flr() {return NULL; }
Number* Complex::cel() {return NULL; }
Number* Complex::trc() {return NULL; }
Number* Complex::rnd() {return NULL; }
Number* Complex::maxi(Number *number2) {return NULL; }
Number* Complex::mini(Number *number2) {return NULL; }

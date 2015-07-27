#include "complex.h"
#include "boolean.h"
#include <complex>
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
                *tmp2 = *SCAST_FLOAT(tmp2->convert(tmp->imag_));
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
        tmp_real_->print();

        Rational* tmp_imag_ = SCAST_RATIONAL(imag_);
        int FLAG=BigInt::abs_cmp(tmp_imag_->num_.number_,"0");
        if (FLAG==1 && !tmp_imag_->num_.sgn_) printf("+");
        else if (FLAG==0) return;
        if (tmp_imag_->num_.number_ == "1" && tmp_imag_->den_.number_ == "1" )
        {
            if (!tmp_imag_->num_.sgn_) { printf("i"); return; }
            else {printf("-i"); return; }
        }
        tmp_imag_->print();
    }
    else
    {
        Float* tmp_real_ = SCAST_FLOAT(real_);
        tmp_real_->print();

        Float* tmp_imag_ = SCAST_FLOAT(imag_);
        if (tmp_imag_->number_>=0)
            printf("+");
        tmp_imag_->print();
    }
	printf("i");
}

Number* Complex::abss()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "abs can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->abss();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::quo(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    Rational* tmp1_real = SCAST_RATIONAL(real_);
    Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "quotient is only for integers");
    assert(tmp1_real->den_ == ONE_ && tmp2_real->den_ == ONE_
           && "quotient is only for integers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->quo(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::rem(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    Rational* tmp1_real = SCAST_RATIONAL(real_);
    Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "remainder is only for integers");
    assert(tmp1_real->den_ == ONE_ && tmp2_real->den_ == ONE_
           && "remainder is only for integers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->rem(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::mod(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    Rational* tmp1_real = SCAST_RATIONAL(real_);
    Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "modulo is only for integers");
    assert(tmp1_real->den_ == ONE_ && tmp2_real->den_ == ONE_
           && "modulo is only for integers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->mod(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::gcd(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "gcd is only for rational numbers");
    //assert(real_->den_ == ONE_ && tmp2->real_->den_ == ONE_
           //&& "gcd is only for integers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->gcd(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::lcm(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "lcm is only for rational numbers");
    //assert(real_->den_ == ONE_ && tmp2->real_->den_ == ONE_
           //&& "lcm is only for integers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->lcm(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::expp(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    Complex* a = SCAST_COMPLEX(exttoinext());
    Complex* b = SCAST_COMPLEX(tmp2->exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_b(SCAST_FLOAT(b->real_)->number_,SCAST_FLOAT(b->imag_)->number_);
    complex<double> c_res = exp(c_b*log(c_a));
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    delete b;
    return res;
}

Number* Complex::expe()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = exp(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;
}

Number* Complex::logg()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = log(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;
}

Number* Complex::sqt()
{
    Complex* C_HALF_ = new Complex;
    Number* pre_real = C_HALF_->real_;
    Number* pre_imag = C_HALF_->imag_;
    C_HALF_->real_ = new Float(0.5);
    C_HALF_->imag_ = new Float(0.0);
    C_HALF_->exact_ = false;
    delete pre_real;
    delete pre_imag;
    return expp(C_HALF_);
}

Number* Complex::flr()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "floor can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->flr();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::cel()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "ceiling can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->cel();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::trc()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "abs can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->trc();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::rnd()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "abs can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->rnd();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::maxi(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "max is only for rational numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->maxi(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::mini(Number *number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "min is only for rational numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->mini(tmp2->real_);
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::numpart()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "numerator can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->numpart();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::denpart()
{
    assert(exact_ && "inexact complex number!");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    assert(tmp1_imag->num_ == ZERO_ && "numerator can't be used in complex numbers");
    Complex* res = new Complex;
    Number *preal=res->real_, *pimag=res->imag_;
    res->real_ = real_->denpart();
    res->imag_ = new Rational(ZERO_,ONE_);
    res->exact_ = true;
    delete preal;
    delete pimag;
    return res;
}

Number* Complex::rpart()
{
    if (exact_)
    {
        Rational* tmp = SCAST_RATIONAL(real_);
        return new Rational(*tmp);
    }
    else
    {
        Float* tmp = SCAST_FLOAT(real_);
        return new Float(*tmp);
    }
}

Number* Complex::ipart()
{
    if (exact_)
    {
        Rational* tmp = SCAST_RATIONAL(imag_);
        return new Rational(*tmp);
    }
    else
    {
        Float* tmp = SCAST_FLOAT(imag_);
        return new Float(*tmp);
    }
}

Number* Complex::makeRec(Number* number2)
{
    assert(exact_ && "make-rectangular is only for real");
    Complex* tmp2 = SCAST_COMPLEX(number2);
    Rational* tmp1_real = SCAST_RATIONAL(real_);
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "make-rectangular is only for real");
    Complex* res = new Complex;
    res->exact_ = true;
    res->real_ = new Rational(tmp1_real->num_,tmp1_real->den_);
    res->imag_ = new Rational(tmp2_real->num_,tmp2_real->den_);
    return res;

}

Number* Complex::makePol(Number* number2)
{
    assert(exact_ && "make-rectangular is only for real");
    Complex* tmp2 = SCAST_COMPLEX(number2);
    Rational* tmp1_real = SCAST_RATIONAL(real_);
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_ == ZERO_ && tmp2_imag->num_ == ZERO_
           && "make-rectangular is only for real");
    Float* tmp = new Float;
    Float* f_tmp1 = SCAST_FLOAT(tmp->convert(tmp1_real));
    Float* f_tmp2 = SCAST_FLOAT(tmp->convert(tmp2_real));
    complex<double> c_res = polar(f_tmp1->number_,f_tmp2->number_);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete f_tmp1;
    delete f_tmp2;
    return res;
}

Number* Complex::magnt()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    Float* res = new Float(abs(c_a));
    delete a;
    return res;
}

Number* Complex::ang()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    Float* res = new Float(arg(c_a));
    delete a;
    return res;
}

SchemeUnit* Complex::isExact() { return new Boolean(exact_); }
SchemeUnit* Complex::isInexact() { return new Boolean(!exact_); }

Number* Complex::exttoinext()
{
    if (!exact_)
    {
        Complex* res = new Complex;
        res->real_ = new Float(*SCAST_FLOAT(real_));
        res->imag_ = new Float(*SCAST_FLOAT(imag_));
        res->exact_=false;
        return res;
    }

    Complex* res = new Complex;

    Float* tmp1 = new Float;
    *tmp1 = *SCAST_FLOAT(tmp1->convert(real_));
    Number* pre_real = res->real_;
    res->real_ = tmp1;
    delete pre_real;

    Float* tmp2 = new Float;
    *tmp2 = *SCAST_FLOAT(tmp2->convert(imag_));
    Number* pre_imag = res->imag_;
    res->imag_ = tmp2;
    delete pre_imag;

    res->exact_ = !exact_;
    return res;

}

Number* Complex::inexttoext()
{
    if (exact_)
    {
        Complex* res = new Complex;
        res->real_ = new Rational(*SCAST_RATIONAL(real_));
        res->imag_ = new Rational(*SCAST_RATIONAL(imag_));
        res->exact_=true;
        return res;
    }

    Complex* res = new Complex;

    Number* tmp1;
    Number* pre_real = res->real_;
    tmp1 = real_->inexttoext();
    res->real_ = tmp1;
    delete pre_real;

    Number* tmp2;
    Number* pre_imag = res->imag_;
    tmp2 = imag_->inexttoext();
    res->imag_ = tmp2;
    delete pre_imag;

    res->exact_ = !exact_;
    return res;
}

Number* Complex::sinn()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = sin(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;

}

Number* Complex::coss()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = cos(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;

}

Number* Complex::tann()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = tan(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;

}

Number* Complex::asinn()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = asin(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;

}

Number* Complex::acoss()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = acos(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;

}

Number* Complex::atann()
{
    Complex* a = SCAST_COMPLEX(exttoinext());
    complex<double> c_a(SCAST_FLOAT(a->real_)->number_,SCAST_FLOAT(a->imag_)->number_);
    complex<double> c_res = atan(c_a);
    Complex* res = new Complex;
    res->exact_ = false;
    res->real_ = new Float(::real(c_res));
    res->imag_ = new Float(::imag(c_res));
    delete a;
    return res;

}

Number* Complex::atann(Number* number2)
{
    return (number2->makeRec(this))->ang();
}

SchemeUnit* Complex::eql(Number* number2)
{
    Complex* dif = SCAST_COMPLEX(sub(number2));
    if (exact_)
    {
        Rational* dif_real = SCAST_RATIONAL(dif->real_);
        Rational* dif_imag = SCAST_RATIONAL(dif->imag_);
        return new Boolean(dif_real->num_==ZERO_ && dif_imag->num_==ZERO_);
    }
    else
    {
        Float* dif_real = SCAST_FLOAT(dif->real_);
        Float* dif_imag = SCAST_FLOAT(dif->imag_);
        return new Boolean(fabs(dif_real->number_-nearbyint(dif_real->number_))<1e-300
                           && fabs(dif_imag->number_-nearbyint(dif_imag->number_))<1e-300);
    }

}

SchemeUnit* Complex::moInc(Number* number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "< is only for real");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_==ZERO_ && tmp2_imag->num_==ZERO_
           && "< is only for real");
    return real_->moInc(tmp2->real_);
}

SchemeUnit* Complex::nonDec(Number* number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "< is only for real");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_==ZERO_ && tmp2_imag->num_==ZERO_
           && "< is only for real");
    return real_->nonDec(tmp2->real_);
}

SchemeUnit* Complex::moDec(Number* number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "< is only for real");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_==ZERO_ && tmp2_imag->num_==ZERO_
           && "< is only for real");
    return real_->moDec(tmp2->real_);
}

SchemeUnit* Complex::nonInc(Number* number2)
{
    Complex* tmp2 = SCAST_COMPLEX(number2);
    assert(exact_ && tmp2->exact_ && "< is only for real");
    Rational* tmp1_imag = SCAST_RATIONAL(imag_);
    Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
    assert(tmp1_imag->num_==ZERO_ && tmp2_imag->num_==ZERO_
           && "< is only for real");
    return real_->nonInc(tmp2->real_);
}

SchemeUnit* Complex::isZero()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        if (tmp_real->num_!=ZERO_) return new Boolean(false);
        else return new Boolean(true);
    }
    else
    {
        Float* tmp_real = SCAST_FLOAT(real_);
        Float* tmp_imag = SCAST_FLOAT(imag_);
        if (fabs(tmp_imag->number_)>1e-300) return new Boolean(false);
        if (fabs(tmp_real->number_)>1e-300) return new Boolean(false);
        else return new Boolean(true);
    }
}

SchemeUnit* Complex::isNegative()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        if (!tmp_real->num_.sgn_) return new Boolean(false);
        else return new Boolean(true);
    }
    else return new Boolean(false);
}

SchemeUnit* Complex::isPositive()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        if (tmp_real->num_.sgn_ || tmp_real->num_==ZERO_) return new Boolean(false);
        else return new Boolean(true);
    }
    else return new Boolean(false);
}

SchemeUnit* Complex::isOdd()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        return tmp_real->isOdd();
    }
    else return new Boolean(false);
}

SchemeUnit* Complex::isEven()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        return tmp_real->isEven();
    }
    else return new Boolean(false);
}

SchemeUnit* Complex::isInteger()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        if (tmp_real->den_!=ONE_) return new Boolean(false);
        else return new Boolean(true);
    }
    else
    {
        Float* tmp_real = SCAST_FLOAT(real_);
        Float* tmp_imag = SCAST_FLOAT(imag_);
        if (fabs(tmp_imag->number_)>1e-300) return new Boolean(false);
        if (fabs(tmp_real->number_-nearbyint(tmp_real->number_))>1e-300)
            return new Boolean(false);
        else return new Boolean(true);
    }
}

SchemeUnit* Complex::isRational()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        else return new Boolean(true);
    }
    else
    {
        Float* tmp_real = SCAST_FLOAT(real_);
        Float* tmp_imag = SCAST_FLOAT(imag_);
        if (fabs(tmp_imag->number_)>1e-300) return new Boolean(false);
        else return new Boolean(true);
    }
}

SchemeUnit* Complex::isReal()
{
    if (exact_)
    {
        Rational* tmp_real = SCAST_RATIONAL(real_);
        Rational* tmp_imag = SCAST_RATIONAL(imag_);
        if (tmp_imag->num_!=ZERO_) return new Boolean(false);
        else return new Boolean(true);
    }
    else
    {
        Float* tmp_real = SCAST_FLOAT(real_);
        Float* tmp_imag = SCAST_FLOAT(imag_);
        if (fabs(tmp_imag->number_)>1e-300) return new Boolean(false);
        else return new Boolean(true);
    }
}

SchemeUnit* Complex::isComplex()
{
    return new Boolean(true);
}

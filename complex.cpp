#include "complex.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#define ABS(x) ((x)<0?(-(x)):(x))
Complex::Complex(Number* real=NULL, Number* imag=NULL): real_(real), imag_(imag)
{
    type_=COMPLEX;
    //exact_ = 1;
    //if ()
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
	if (exact_)
    {
        Rational* tmp1_real_ = SCAST_RATIONAL(real_);
        Rational* tmp1_imag_ = SCAST_RATIONAL(imag_);
        Rational* tmp2_real_ = SCAST_RATIONAL(tmp2->real_);
        Rational* tmp2_imag_ = SCAST_RATIONAL(tmp2->imag_);
        result->real_ = tmp1_real_->add(tmp2_real_);
        result->imag_ = tmp1_imag_->add(tmp2_imag_);
        result->exact_ = exact_;
    }
    else
    {
        /*cout<<endl;
        cout<<"in add func: ";*/
        Float* tmp1_real_ = SCAST_FLOAT(real_);
        Float* tmp1_imag_ = SCAST_FLOAT(imag_);
        Float* tmp2_real_ = SCAST_FLOAT(tmp2->real_);
        Float* tmp2_imag_ = SCAST_FLOAT(tmp2->imag_);
        result->real_ = tmp1_real_->add(tmp2_real_);
        result->imag_ = tmp1_imag_->add(tmp2_imag_);
        result->exact_ = exact_;
    }
    /*cout<<endl;
    cout<<"in add func final: ";
    result->print();*/
	return result;
}

Number* Complex::sub(Number* number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	if (exact_)
    {
        Rational* tmp1_real_ = SCAST_RATIONAL(real_);
        Rational* tmp1_imag_ = SCAST_RATIONAL(imag_);
        Rational* tmp2_real_ = SCAST_RATIONAL(tmp2->real_);
        Rational* tmp2_imag_ = SCAST_RATIONAL(tmp2->imag_);
        result->real_ = tmp1_real_->sub(tmp2_real_);
        result->imag_ = tmp1_imag_->sub(tmp2_imag_);
        result->exact_ = exact_;
    }
    else
    {
        Float* tmp1_real_ = SCAST_FLOAT(real_);
        Float* tmp1_imag_ = SCAST_FLOAT(imag_);
        Float* tmp2_real_ = SCAST_FLOAT(tmp2->real_);
        Float* tmp2_imag_ = SCAST_FLOAT(tmp2->imag_);
        result->real_ = tmp1_real_->sub(tmp2_real_);
        result->imag_ = tmp1_imag_->sub(tmp2_imag_);
        result->exact_ = exact_;
    }
	return result;
}

Number* Complex::mul(Number* number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	if (exact_)
    {
        Rational* tmp1_real_ = SCAST_RATIONAL(real_);
        Rational* tmp1_imag_ = SCAST_RATIONAL(imag_);
        Rational* tmp2_real_ = SCAST_RATIONAL(tmp2->real_);
        Rational* tmp2_imag_ = SCAST_RATIONAL(tmp2->imag_);
        result->real_ = SCAST_RATIONAL(tmp1_real_->mul(tmp2_real_))->sub(SCAST_RATIONAL(tmp1_imag_->mul(tmp2_imag_)));
        result->imag_ = SCAST_RATIONAL(tmp1_real_->mul(tmp2_imag_))->add(SCAST_RATIONAL(tmp1_imag_->mul(tmp2_real_)));
        result->exact_ = exact_;
    }
    else
    {
        Float* tmp1_real_ = SCAST_FLOAT(real_);
        Float* tmp1_imag_ = SCAST_FLOAT(imag_);
        Float* tmp2_real_ = SCAST_FLOAT(tmp2->real_);
        Float* tmp2_imag_ = SCAST_FLOAT(tmp2->imag_);
        result->real_ = SCAST_FLOAT(tmp1_real_->mul(tmp2_real_))->sub(SCAST_FLOAT(tmp1_imag_->mul(tmp2_imag_)));
        result->imag_ = SCAST_FLOAT(tmp1_real_->mul(tmp2_imag_))->add(SCAST_FLOAT(tmp1_imag_->mul(tmp2_real_)));
        result->exact_ = exact_;
    }
	return result;
}

Number* Complex::div(Number* number2)
{
	Complex* tmp2 = SCAST_COMPLEX(number2);
	Complex* result = new Complex;
	//assert( tmp2->real_->num_!=ZERO_ && "divided by zero");
	if (exact_)
    {
        Rational* tmp1_real_ = SCAST_RATIONAL(real_);
        Rational* tmp1_imag_ = SCAST_RATIONAL(imag_);
        Rational* tmp2_real_ = SCAST_RATIONAL(tmp2->real_);
        Rational* tmp2_imag_ = SCAST_RATIONAL(tmp2->imag_);
        /*ENTER;
        tmp2_real_->print();
        ENTER;
        tmp2_imag_->print();
        */
        Rational* sqrsum;
        sqrsum = SCAST_RATIONAL(SCAST_RATIONAL(tmp2_real_->mul(tmp2_real_))->add(SCAST_RATIONAL(tmp2_imag_->mul(tmp2_imag_))));

        /*ENTER;
        (tmp2_real_->mul(tmp2_real_))->print();
        ENTER;
        (tmp2_imag_->mul(tmp2_imag_))->print();
        ENTER;
        cout<<"sqrsum: ";
        sqrsum->print();
        ENTER;*/

        result->real_ = SCAST_RATIONAL(SCAST_RATIONAL(tmp1_real_->mul(tmp2_real_))->add(SCAST_RATIONAL(tmp1_imag_->mul(tmp2_imag_))))->div(sqrsum);
        result->imag_ = SCAST_RATIONAL(SCAST_RATIONAL(tmp1_imag_->mul(tmp2_real_))->sub(SCAST_RATIONAL(tmp1_real_->mul(tmp2_imag_))))->div(sqrsum);
        result->exact_ = exact_;
    }
    else
    {
        Float* tmp1_real_ = SCAST_FLOAT(real_);
        Float* tmp1_imag_ = SCAST_FLOAT(imag_);
        Float* tmp2_real_ = SCAST_FLOAT(tmp2->real_);
        Float* tmp2_imag_ = SCAST_FLOAT(tmp2->imag_);
        Float* sqrsum;
        sqrsum = SCAST_FLOAT(SCAST_FLOAT(tmp2_real_->mul(tmp2_real_))->add(SCAST_FLOAT(tmp2_imag_->mul(tmp2_imag_))));
        result->real_ = SCAST_FLOAT(SCAST_FLOAT(tmp1_real_->mul(tmp2_real_))->add(SCAST_FLOAT(tmp1_imag_->mul(tmp2_imag_))))->div(sqrsum);
        result->imag_ = SCAST_FLOAT(SCAST_FLOAT(tmp1_imag_->mul(tmp2_real_))->sub(SCAST_FLOAT(tmp1_real_->mul(tmp2_imag_))))->div(sqrsum);
        result->exact_ = exact_;
    }
	return result;
}


Complex* Complex::from_string(char* expression)
{
    char* i_pos = strchr(expression, 'i');
    char* sgn_pos;
    Number* real = NULL;
    Number* imag = NULL;
    //Complex* res = new Complex;
    if (i_pos)
    {
        char* tmp = new char [i_pos-expression+2];
        char* pointer=expression;
        int tmp_len=0;
        do
        {
            tmp[tmp_len]=expression[tmp_len];
            ++tmp_len;
        }while (expression[tmp_len]!='+' &&
                expression[tmp_len]!='-' &&
                expression[tmp_len]!='\0');

        //cout<<tmp<<endl;
        bool isExact=1;
        if (tmp[tmp_len-1]!='i')
        {
            //first part convert to real
            tmp[tmp_len]='\0';
            real = Rational::from_string(tmp);
            if (!real) { real=Float::from_string(tmp); isExact=0; }
            if (!real) { delete [] tmp; return NULL; }//if failed

            pointer=expression+tmp_len;
            delete [] tmp;
            tmp = new char [i_pos-expression+2];
            tmp_len = 0;
            do
            {
                tmp[tmp_len]=pointer[tmp_len];
                ++tmp_len;
            }while (pointer[tmp_len]!='+' &&
                    pointer[tmp_len]!='-' &&
                    pointer[tmp_len]!='\0');

            tmp[tmp_len-1]='\0';// pop i
            //cout<<tmp<<endl;
            imag = Rational::from_string(tmp);
            //imag->print();
            if (!imag) { imag=Float::from_string(tmp); isExact=0; }
            if (!imag) { delete [] tmp; return NULL; }
            //imag->print();
            delete [] tmp;
            if (real && imag)
            {
                Complex* res;
                if (!isExact)
                {
                    if (real->type_ > imag->type_)
                    {
                        Number* con_imag=real->convert(imag);
                        delete imag;
                        res = new Complex(real,con_imag);
                    }
                    else if (real->type_ < imag->type_)
                    {
                        Number* con_real=imag->convert(real);
                        delete real;
                        res = new Complex(con_real,imag);
                    }
                    else res = new Complex(real,imag);
                }

                else res = new Complex(real,imag);
                res->exact_ = isExact;
                return res;
            }
            else return NULL;
        }

        else if (tmp[tmp_len-1]=='i')
        {
            tmp[tmp_len-1]='\0';
            imag = Rational::from_string(tmp);
            if (!imag) { imag=Float::from_string(tmp); isExact=0; }
            if (!imag) { delete [] tmp; return NULL; }
            delete [] tmp;

            if (imag)
            {
                Complex* res;
                if (isExact) real = new Rational;
                else real = new Float;
                res = new Complex(real,imag);
                res->exact_ = isExact;

                /*cout<<endl;
                res->print();*/

                return res;
            }
            else return NULL;
        }
    }
    else return NULL;
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
        if (FLAG==1 && real_no_zero && !tmp_imag_->num_.sgn_)
            printf("+");
        else if (FLAG==0)
            return;
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
        else if (ABS(tmp_imag_->number_)<1e-300)
            return;
        tmp_imag_->print();
    }
    //cout<<"FUCK"<<endl;
	printf("i");
}

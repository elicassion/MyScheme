#ifndef compare_h
#define compare_h

#include "opt.h"
#include "float.h"
#include "rational.h"
#include "complex.h"
#include "number.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <complex>
#include <cstdlib>
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)

class Add : public Opt {
    /* Use the lowest level type */
    Number *calc(Cons *con)
    {
		Number *res = new Rational(ZERO_,ONE_);
		Number *last;
        for (; con; con = con->cdr)
        {
			if(con->car->type_>3||con->car->type_<1) { throw 0; }
            Number *opr = con->car, *conv;
            last = res;
            //cout<<"opr type: "<<opr->type_<<endl;
            if (res->type_ > opr->type_)
            {
				res = res->add(conv = res->convert(opr));
			}
			else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->add(conv = res->convert(opr));
            }
            else
			{
                res = (conv = opr->convert(res))->add(opr);
			}

            delete last;
            delete conv;

        }
        return res;
    }
};
class Sub:public Opt{
	Number *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if(tmp->car->type_>3||tmp->car->type_<1)
			{
				throw 0;
			}
			cnt++;
		}
		Number *res=new Rational(ZERO_, ONE_),*last;
		Number *opr = con->car, *conv;
		last=res;
		if(cnt==1)
		{
			if (res->type_ > opr->type_)
            {
                res = res->sub(conv = res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->sub(conv = res->convert(opr));
            }
			else
				res = (conv = opr->convert(res))->sub(opr);
			delete last;
			delete conv;
			return res;
		}
		if (res->type_ > opr->type_)
        {
            res = res->add(conv = res->convert(opr));
        }
        else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
        {
            res = res->add(conv = res->convert(opr));
        }
        else
        {
            res = (conv = opr->convert(res))->add(opr);
        }
        con=con->cdr;
        delete last;
        delete conv;
		for(;con;con=con->cdr)
		{
			opr=con->car;
			last=res;
			if(res->type_>opr->type_)
            {
                res=res->sub(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->sub(conv = res->convert(opr));
            }
			else
            {
                res=(conv=opr->convert(res))->sub(opr);
            }
			delete last;
			delete conv;
		}
		return res;
	}
};
class Mul : public Opt {
    /* Use the lowest level type */
    Number* calc(Cons *con)
    {
        Number *res = new Rational(ONE_, ONE_), *last;
        for (; con; con = con->cdr)
        {
			if(con->car->type_>3||con->car->type_<1)
			{
				throw 0;
			}
            Number *opr = con->car, *conv;
            last = res;
            if (res->type_ > opr->type_)
            {
                res = res->mul(conv = res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->mul(conv = res->convert(opr));
            }
            else
            {
                res = (conv = opr->convert(res))->mul(opr);
            }
            delete last;
            delete conv;
        }
        return res;
    }
};
class Div:public Opt{
	Number *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if(tmp->car->type_>3||tmp->car->type_<1)
			{
				throw 0;
			}
			cnt++;
		}
		Number *res = new Rational(ONE_,ONE_),*last;
		Number *opr = con->car,*conv;
		last=res;
		Number *zero = new Float(0.0);
		if(cnt==1)
		{
			if(res->type_>opr->type_)
            {
                res = res->div(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->div(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->div(opr);
            }
			delete last;
			delete conv;
			return res;
		}
		if(res->type_>opr->type_)
        {
            res = res->mul(conv=res->convert(opr));
        }
        else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
        {
            res = res->mul(conv = res->convert(opr));
        }
		else
        {
            res = (conv=opr->convert(res))->mul(opr);
        }
		con=con->cdr;
		delete last;
		delete conv;
		for(;con;con=con->cdr)
		{
			opr=con->car;
			last=res;
			if(res->type_>opr->type_)
            {
                res = res->div(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->div(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->div(opr);
            }
			delete last;
			delete conv;
		}
		return res;
	}
};


#endif // compare_h

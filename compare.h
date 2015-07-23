#ifndef compare_h
#define compare_h

#include "schemeunit.h"
#include "boolean.h"
#include "opt.h"
#include "float.h"
#include "rational.h"
#include "complex.h"
#include "number.h"
#include "character.h"
#include "mystring.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <complex>
#include <cstdlib>
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_NUMBER(x) static_cast<Number*>(x)

class Add : public Opt {
    /* Use the lowest level type */
    SchemeUnit *calc(Cons *con)
    {
		Number *res = new Rational(ZERO_,ONE_);
		Number *last;
        for (; con; con = con->cdr)
        {
			if (con->car->unitType_!=2) {throw 0;}
			Number *opr = SCAST_NUMBER(con->car), *conv;

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
	SchemeUnit *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		Number *res=new Rational(ZERO_, ONE_),*last;
		Number *opr = SCAST_NUMBER(con->car), *conv;
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
			opr=SCAST_NUMBER(con->car);
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

class Mul:public Opt {
    /* Use the lowest level type */
    SchemeUnit* calc(Cons *con)
    {
        Number *res = new Rational(ONE_, ONE_), *last;
        for (; con; con = con->cdr)
        {
			if (con->car->unitType_!=2) {throw 0;}
			Number *opr = SCAST_NUMBER(con->car), *conv;
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
	SchemeUnit *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		Number *res=new Rational(ONE_, ONE_),*last;
		Number *opr = SCAST_NUMBER(con->car), *conv;
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
			opr=SCAST_NUMBER(con->car);
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

class Abs:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "abs only one parameter");
        Number *opr = SCAST_NUMBER(con->car);
        Number *res = opr->abss();
        return res;
    }
};

class Quo:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>2)
            assert(0 && "quo only two parameter");
        Number *opr1 = SCAST_NUMBER(con->car) , *opr2 = SCAST_NUMBER(con->cdr->car) , *conv;
        Number *res , *last;
        if (opr1->type_ > opr2->type_)
        {
            res = opr1->quo(conv=opr1->convert(opr2));
        }
        else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
        {
            res = opr1->quo(conv = opr1->convert(opr2));
        }
        else
        {
            res = (conv=opr2->convert(opr1))->quo(opr2);
        }
        delete conv;
        return res;
    }
};

class Rem:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>2)
            assert(0 && "remainder only two parameter");
        Number *opr1 = SCAST_NUMBER(con->car) , *opr2 = SCAST_NUMBER(con->cdr->car) , *conv;
        Number *res , *last;
        if (opr1->type_ > opr2->type_)
        {
            res = opr1->rem(conv=opr1->convert(opr2));
        }
        else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
        {
            res = opr1->rem(conv = opr1->convert(opr2));
        }
        else
        {
            res = (conv=opr2->convert(opr1))->rem(opr2);
        }
        delete conv;
        return res;
    }
};

class Mod:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>2)
            assert(0 && "remainder only two parameter");
        Number *opr1 = SCAST_NUMBER(con->car) , *opr2 = SCAST_NUMBER(con->cdr->car) , *conv;
        Number *res , *last;
        if (opr1->type_ > opr2->type_)
        {
            res = opr1->mod(conv=opr1->convert(opr2));
        }
        else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
        {
            res = opr1->mod(conv = opr1->convert(opr2));
        }
        else
        {
            res = (conv=opr2->convert(opr1))->mod(opr2);
        }
        delete conv;
        return res;
    }
};

class Gcd:public Opt{
    SchemeUnit *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt==0)
            return new Rational();
		Number *res = SCAST_NUMBER(con->car),*last;
		Number *opr = SCAST_NUMBER(con->car),*conv;
		last=res;
		if(cnt==1)
		{
			if(res->type_>opr->type_)
            {
                res = res->gcd(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->gcd(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->gcd(opr);
            }
			delete last;
			delete conv;
			return res;
		}
		for(;con;con=con->cdr)
		{
			opr=SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
            {
                res = res->gcd(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->gcd(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->gcd(opr);
            }
			delete last;
			delete conv;
		}
		return res;
	}
};

class Lcm:public Opt{
    SchemeUnit *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt==0)
            return new Rational(ONE_,ONE_);
		Number *res = SCAST_NUMBER(con->car),*last;
		Number *opr = SCAST_NUMBER(con->car),*conv;
		last=res;
		if(cnt==1)
		{
			if(res->type_>opr->type_)
            {
                res = res->lcm(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->lcm(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->lcm(opr);
            }
			delete last;
			delete conv;
			return res;
		}
		for(;con;con=con->cdr)
		{
			opr=SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
            {
                res = res->lcm(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->lcm(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->lcm(opr);
            }
			delete last;
			delete conv;
		}
		return res;
	}
};

class Expt:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>2)
            assert(0 && "expt only two parameter");
        Number *opr1 = SCAST_NUMBER(con->car) , *opr2 = SCAST_NUMBER(con->cdr->car) , *conv;
        Number *res , *last;
        if (opr1->type_ > opr2->type_)
        {
            res = opr1->expp(conv=opr1->convert(opr2));
        }
        else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
        {
            res = opr1->expp(conv = opr1->convert(opr2));
        }
        else
        {
            res = (conv=opr2->convert(opr1))->expp(opr2);
        }
        delete conv;
        return res;
    }
};

class Exp:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "exp only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->expe() , *last;
        return res;
    }
};

class Log:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "exp only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->logg() , *last;
        return res;
    }
};

class Sqrt:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "sqrt only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->sqt() , *last;
        return res;
    }
};

class Flr:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "floor only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->flr() , *last;
        return res;
    }
};

class Cel:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "ceiling only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->cel() , *last;
        return res;
    }
};

class Trc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "truncate only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->trc() , *last;
        return res;
    }
};

class Rnd:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "round only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->rnd() , *last;
        return res;
    }
};

class Maxi:public Opt{
    SchemeUnit *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt==0)
            assert(0 && "parameter empty");
		Number *res = SCAST_NUMBER(con->car),*last;
		Number *opr = SCAST_NUMBER(con->car),*conv;
		last=res;
		if(cnt==1)
		{
			if(res->type_>opr->type_)
            {
                res = res->maxi(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->maxi(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->maxi(opr);
            }
			delete last;
			delete conv;
			return res;
		}
		for(;con;con=con->cdr)
		{
			opr=SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
            {
                res = res->maxi(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->maxi(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->maxi(opr);
            }
			delete last;
			delete conv;
		}
		return res;
	}
};

class Mini:public Opt{
    SchemeUnit *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt==0)
            assert(0 && "parameter empty");
		Number *res = SCAST_NUMBER(con->car),*last;
		Number *opr = SCAST_NUMBER(con->car),*conv;
		last=res;
		if(cnt==1)
		{
			if(res->type_>opr->type_)
            {
                res = res->mini(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->mini(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->mini(opr);
            }
			delete last;
			delete conv;
			return res;
		}
		for(;con;con=con->cdr)
		{
			opr=SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
            {
                res = res->mini(conv=res->convert(opr));
            }
            else if (res->type_ == opr->type_ && res->exact_ < opr->exact_)
            {
                res = res->mini(conv = res->convert(opr));
            }
			else
            {
                res = (conv=opr->convert(res))->mini(opr);
            }
			delete last;
			delete conv;
		}
		return res;
	}
};

class Numpart:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "numerator only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->numpart() , *last;
        return res;
    }
};

class Denpart:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "denominator only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->denpart() , *last;
        return res;
    }
};

class Rpart:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "real-part only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->rpart() , *last;
        return res;
    }
};

class Ipart:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "imag-part only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->ipart() , *last;
        return res;
    }
};

class MakeRec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>2)
            assert(0 && "make-rectangular only two parameter");
        Number *opr1 = SCAST_NUMBER(con->car) , *opr2 = SCAST_NUMBER(con->cdr->car) , *conv;
        assert((opr1->type_ <=3 && opr2->type_ <=3)
               && "make-rectangular is only for real");
        Number *res , *last;
        if (opr1->type_ > opr2->type_)
        {
            res = opr1->makeRec(conv=opr1->convert(opr2));
        }
        else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
        {
            res = opr1->makeRec(conv = opr1->convert(opr2));
        }
        else
        {
            res = (conv=opr2->convert(opr1))->makeRec(opr2);
        }
        delete conv;
        return res;
    }
};

class MakePol:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>2)
            assert(0 && "make-polar only two parameter");
        Number *opr1 = SCAST_NUMBER(con->car) , *opr2 = SCAST_NUMBER(con->cdr->car) , *conv;
        assert((opr1->type_ <=2 && opr2->type_ <=2)
               && "make-polar is only for real");
        Number *res , *last;
        if (opr1->type_ > opr2->type_)
        {
            res = opr1->makePol(conv=opr1->convert(opr2));
        }
        else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
        {
            res = opr1->makePol(conv = opr1->convert(opr2));
        }
        else
        {
            res = (conv=opr2->convert(opr1))->makePol(opr2);
        }
        delete conv;
        return res;
    }
};

class Magnt:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "magnitude only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->magnt() , *last;
        return res;
    }
};

class Ang:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "angle only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->ang() , *last;
        return res;
    }
};

class Isexact:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "exact? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isExact() , *last;
        return res;
    }
};

class IsInexact:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "inexact? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isInexact() , *last;
        return res;
    }
};

class Exttoinext:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "exact to inexact only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->exttoinext() , *last;
        return res;
    }
};

class Inexttoext:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "inexact to exact only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->inexttoext() , *last;
        return res;
    }
};

class Sinn:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "sin only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->sinn() , *last;
        return res;
    }
};

class Coss:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "cos only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->coss() , *last;
        return res;
    }
};

class Tann:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "tan only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->tann() , *last;
        return res;
    }
};

class Asinn:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "asin only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->asinn() , *last;
        return res;
    }
};

class Acoss:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "acos only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->acoss() , *last;
        return res;
    }
};

class Atann:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "atan only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        Number *res = opr->atann() , *last;
        return res;
    }
};

class Eql:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt<2)
            assert(0 && "= parameter at least 2");
		SchemeUnit *res = new Boolean,*last;
		Number *opr1;
		Number *opr2 ,*conv;
		do
		{
			last=res;
			opr1 = SCAST_NUMBER(con->car);
			opr2 = SCAST_NUMBER(con->cdr->car);
			con=con->cdr;
			if(opr1->type_>opr2->type_)
            {
                res = opr1->eql(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->eql(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->eql(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class MoInc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt<2)
            assert(0 && "< parameter at least 2");
		SchemeUnit *res = new Boolean,*last;
		Number *opr1;
		Number *opr2 ,*conv;
		do
		{
			last=res;
			opr1 = SCAST_NUMBER(con->car);
			opr2 = SCAST_NUMBER(con->cdr->car);
			con=con->cdr;
			if(opr1->type_>opr2->type_)
            {
                res = opr1->moInc(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->moInc(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->moInc(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class NonDec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt<2)
            assert(0 && "<= parameter at least 2");
		SchemeUnit *res = new Boolean,*last;
		Number *opr1;
		Number *opr2 ,*conv;
		do
		{
			last=res;
			opr1 = SCAST_NUMBER(con->car);
			opr2 = SCAST_NUMBER(con->cdr->car);
			con=con->cdr;
			if(opr1->type_>opr2->type_)
            {
                res = opr1->nonDec(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->nonDec(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->nonDec(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class MoDec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt<2)
            assert(0 && "> parameter at least 2");
		SchemeUnit *res = new Boolean,*last;
		Number *opr1;
		Number *opr2 ,*conv;
		do
		{
			last=res;
			opr1 = SCAST_NUMBER(con->car);
			opr2 = SCAST_NUMBER(con->cdr->car);
			con=con->cdr;
			if(opr1->type_>opr2->type_)
            {
                res = opr1->moDec(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->moDec(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->moDec(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class NonInc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt<2)
            assert(0 && ">= parameter at least 2");
		SchemeUnit *res = new Boolean,*last;
		Number *opr1;
		Number *opr2 ,*conv;
		do
		{
			last=res;
			opr1 = SCAST_NUMBER(con->car);
			opr2 = SCAST_NUMBER(con->cdr->car);
			con=con->cdr;
			if(opr1->type_>opr2->type_)
            {
                res = opr1->nonInc(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->nonInc(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->nonInc(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class Nott:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_>4) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "not only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->nott() , *last;
        return res;
    }
};

class IsZero:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "zero? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isZero() , *last;
        return res;
    }
};

class IsNegative:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "negative? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isNegative() , *last;
        return res;
    }
};

class IsPositive:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "positive? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isPositive() , *last;
        return res;
    }
};

class IsOdd:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "odd? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isOdd() , *last;
        return res;
    }
};

class IsEven:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_!=2) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "even? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isEven() , *last;
        return res;
    }
};

class IsInteger:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_>4) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "integer? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isInteger() , *last;
        return res;
    }
};

class IsRational:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_>4) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "rational? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isRational() , *last;
        return res;
    }
};

class IsReal:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_>4) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "real? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isReal() , *last;
        return res;
    }
};

class IsComplex:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_>4) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "complex? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isComplex() , *last;
        return res;
    }
};

class IsNumber:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if (tmp->car->unitType_>4) {throw 0;}
			cnt++;
		}
		if (cnt>1)
            assert(0 && "number? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isNumber() , *last;
        return res;
    }
};

class IsChar:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_>4) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isChar() , *last;
        return res;
    }
};

class IsString:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_>4) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "string? only one parameter");
        SchemeUnit *opr = con->car , *conv;
        SchemeUnit *res = opr->isString() , *last;
        return res;
    }
};

class ChEql:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char=? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chEql(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChMoInc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char<? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chMoInc(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChNonDec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char<=? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chNonDec(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChMoDec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char>? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chMoDec(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChNonInc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char>=? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chNonInc(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChCiEql:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char-ci=? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chCiEql(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChCiMoInc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char-ci<? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chCiMoInc(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChCiNonDec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char-ci<=? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chCiNonDec(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChCiMoDec:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char-ci>? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chCiMoDec(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChCiNonInc:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt<2)
            assert(0 && "char-ci>=? parameter at least 2");
        SchemeUnit *res = new Boolean,*last;
        Character *opr1;
        Character *opr2;
        do
        {
            last=res;
            opr1 = SCAST_CHARACTER(con->car);
            opr2 = SCAST_CHARACTER(con->cdr->car);
            con=con->cdr;
            res = opr1->chCiNonInc(opr2);
            delete last;
        }while(con->cdr);
        return res;
    }
};

class ChIsAlpha:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-alphabetic? only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->isChar();
        return res;
    }
};

class ChIsNum:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-numeric? only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chIsNum();
        return res;
    }
};

class ChIsSpace:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-whitespace? only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chIsSpace();
        return res;
    }
};

class ChIsUpCase:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-upper-case? only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chIsUpCase();
        return res;
    }
};

class ChIsLowCase:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-lower-case? only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chIsLowCase();
        return res;
    }
};

class ChToInt:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char->integer only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chToInt();
        return res;
    }
};

class IntToCh:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=2) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "integer->char only one parameter");
        Number *opr = SCAST_NUMBER(con->car);
        SchemeUnit *res = opr->intToCh();
        return res;
    }
};

class ChToUp:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-upcase only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chToUp();
        return res;
    }
};

class ChToDown:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt>1)
            assert(0 && "char-downcase only one parameter");
        Character *opr = SCAST_CHARACTER(con->car);
        SchemeUnit *res = opr->chToDown();
        return res;
    }
};

class MakeString:public Opt{
    SchemeUnit* calc (Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_>3) {throw 0;}
            cnt++;
        }
        if (cnt>2 || cnt<1)
            assert(0 && "make-string only one or two parameters");
        else if (cnt == 2)
        {
            SchemeUnit* opr1 = con->car;
            SchemeUnit* opr2 = con->cdr->car;
            if (opr1->unitType_!=2 && opr2->unitType_!=3)
                assert(0 && "make-string parameter type error");
            Number* tmp1 = SCAST_NUMBER(opr1);
            Character* tmp2 = SCAST_CHARACTER(opr2);
            SchemeUnit* res = String::makeString(tmp1,tmp2);
            return res;
        }
        else if (cnt == 1)
        {
            SchemeUnit* opr1 = con->car;
            if (opr1->unitType_!=2)
                assert(0 && "make-string parameter type error");
            Number* tmp1 = SCAST_NUMBER(opr1);
            Character* tmp2 = new Character('\0');
            SchemeUnit* res = String::makeString(tmp1,tmp2);
            return res;
        }
    }
};

class FormString:public Opt{
    SchemeUnit *calc(Cons *con)
    {
        Cons *tmp=con;
        int cnt=0;
        for(;tmp;tmp=tmp->cdr)
        {
            if (tmp->car->unitType_!=3) {throw 0;}
            cnt++;
        }
        if (cnt==0)
            assert(0 && "parameter empty");
        String *res = new String(""),*last;
        Character *opr;
        for(;con;con=con->cdr)
        {
            opr=SCAST_CHARACTER(con->car);
            last=res;
            res = res->formString(opr);
            delete last;
        }
        return res;
    }
};

#endif // compare_h

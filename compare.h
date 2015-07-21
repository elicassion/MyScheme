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
            assert(0 && "parameter at least 2");
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

class Monoinc:public Opt{
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
            assert(0 && "parameter at least 2");
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
                res = opr1->monoinc(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->monoinc(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->monoinc(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class Mononondec:public Opt{
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
            assert(0 && "parameter at least 2");
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
                res = opr1->mononondec(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->mononondec(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->mononondec(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class Monodec:public Opt{
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
            assert(0 && "parameter at least 2");
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
                res = opr1->monodec(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->monodec(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->monodec(opr2);
            }
			delete last;
			delete conv;
		}while(con->cdr);
		return res;
    }
};

class Monononinc:public Opt{
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
            assert(0 && "parameter at least 2");
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
                res = opr1->monononinc(conv=opr1->convert(opr2));
            }
            else if (opr1->type_ == opr2->type_ && opr1->exact_ < opr2->exact_)
            {
                res = opr1->monononinc(conv = opr1->convert(opr2));
            }
			else
            {
                res = (conv=opr2->convert(opr1))->monononinc(opr2);
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
            assert(0 && "even? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
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
            assert(0 && "even? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
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
            assert(0 && "even? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
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
            assert(0 && "even? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
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
            assert(0 && "even? only one parameter");
        Number *opr = SCAST_NUMBER(con->car) , *conv;
        SchemeUnit *res = opr->isNumber() , *last;
        return res;
    }
};


#endif // compare_h

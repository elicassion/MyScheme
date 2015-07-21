#include "calc_exp.h"
#include "opt.h"
#include "compare.h"
#include "next_token.h"
#include <cstdio>

SchemeUnit *calc_exp()
{
    char *tk0 = next_token();
    SchemeUnit *res;
    if (*tk0 == '(')
    {
        char *tk1 = next_token();
        Opt *opt;
        Cons *cons = new Cons(NULL, NULL), *tail = cons;
        SchemeUnit *val;
		if(tk1==NULL){printf("123");throw 0;}
		if(strcmp(tk1,"+")==0)opt=new Add();
		else if(strcmp(tk1,"-")==0)opt=new Sub();
		else if(strcmp(tk1,"*")==0)opt=new Mul();
		else if(strcmp(tk1,"/")==0)opt=new Div();
		else if(strcmp(tk1,"abs")==0)opt=new Abs();
		else if(strcmp(tk1,"quotient")==0)opt=new Quo();
		else if(strcmp(tk1,"remainder")==0)opt=new Rem();
		else if(strcmp(tk1,"modulo")==0)opt=new Mod();
		else if(strcmp(tk1,"gcd")==0)opt=new Gcd();
		else if(strcmp(tk1,"lcm")==0)opt=new Lcm();
		else if(strcmp(tk1,"expt")==0)opt=new Expt();
		else if(strcmp(tk1,"sqrt")==0)opt=new Sqrt();
		else if(strcmp(tk1,"floor")==0)opt=new Flr();
		else if(strcmp(tk1,"ceiling")==0)opt=new Cel();
		else if(strcmp(tk1,"truncate")==0)opt=new Trc();
		else if(strcmp(tk1,"round")==0)opt=new Rnd();
		else if(strcmp(tk1,"max")==0)opt=new Maxi();
		else if(strcmp(tk1,"min")==0)opt=new Mini();
		else if(strcmp(tk1,"numerator")==0)opt=new Numpart();
		else if(strcmp(tk1,"denominator")==0)opt=new Denpart();
		else if(strcmp(tk1,"real-part")==0)opt=new Rpart();
		else if(strcmp(tk1,"imag-part")==0)opt=new Ipart();
		else if(strcmp(tk1,"exact?")==0)opt=new Isexact();
		else if(strcmp(tk1,"inexact?")==0)opt=new IsInexact();
		else if(strcmp(tk1,"exact->inexact")==0)opt=new Exttoinext();
		else if(strcmp(tk1,"inexact->exact")==0)opt=new Inexttoext();
		else if(strcmp(tk1,"sin")==0)opt=new Sinn();
		else if(strcmp(tk1,"cos")==0)opt=new Coss();
		else if(strcmp(tk1,"tan")==0)opt=new Tann();
		else if(strcmp(tk1,"asin")==0)opt=new Asinn();
		else if(strcmp(tk1,"acos")==0)opt=new Acoss();
		else if(strcmp(tk1,"atan")==0)opt=new Atann();
		else if(strcmp(tk1,"=")==0)opt=new Eql();
		else if(strcmp(tk1,"<")==0)opt=new Monoinc();
		else if(strcmp(tk1,"<=")==0)opt=new Mononondec();
		else if(strcmp(tk1,">")==0)opt=new Monodec();
		else if(strcmp(tk1,">=")==0)opt=new Monononinc();
		else if(strcmp(tk1,"not")==0)opt=new Nott();
		else if(strcmp(tk1,"zero?")==0)opt=new IsZero();
		else if(strcmp(tk1,"negative?")==0)opt=new IsNegative();
		else if(strcmp(tk1,"positive?")==0)opt=new IsPositive();
		else if(strcmp(tk1,"odd?")==0)opt=new IsOdd();
		else if(strcmp(tk1,"even?")==0)opt=new IsEven();
		else if(strcmp(tk1,"integer?")==0)opt=new IsInteger();
		else if(strcmp(tk1,"rational?")==0)opt=new IsRational();
		else if(strcmp(tk1,"real?")==0)opt=new IsReal();
		else if(strcmp(tk1,"complex?")==0)opt=new IsComplex();
		else if(strcmp(tk1,"number?")==0)opt=new IsNumber();
		else throw 0;
        while ((val = calc_exp()))
        {
            tail->cdr = new Cons(val, NULL);
            tail = tail->cdr;
        }
        res = opt->calc(cons->cdr);
        for (Cons *np; cons; cons = np)
        {
            np = cons->cdr;
            delete cons;
        }
    }
    else if (*tk0 == ')')
	{
        return NULL;
	}
    else
    {
		res = Character::from_string(tk0);
		if(!res) res = Rational::from_string(tk0);
		if(!res) res = Float::from_string(tk0);
		if(!res) res = Complex::from_string(tk0);
		if(res==NULL){throw 0;}
    }
    return res;
}

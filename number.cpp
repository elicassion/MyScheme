#include "number.h"
#include "boolean.h"
#include "character.h"
#include "rational.h"
#include "bigint.h"
#include "mystring.h"
#include "complex.h"
#include <cassert>
#include <iostream>
#include <sstream>
using namespace std;
Number::Number()
{
    unitType_ = NUMBER;
}

SchemeUnit* Number::nott()
{
    return new Boolean(false);
}

SchemeUnit* Number::isNumber()
{
    return new Boolean(true);
}

SchemeUnit* Number::isChar()
{
	return new Boolean(false);
}

SchemeUnit* Number::isString()
{
	return new Boolean(false);
}

SchemeUnit* Number::intToCh()
{
	assert(type_==1 && "integer->char is only for integer");
	Rational* tmp = SCAST_RATIONAL(this);
	assert(tmp->den_ == ONE_ && "integer->char is only for integer");
	BigInt MAXCHAR("256");
	assert((tmp->num_ < MAXCHAR || tmp->num_<ZERO_) && "given integer over scope");
	char k = (char)(int)(double)tmp->num_;
	return new Character(k);
}

SchemeUnit* Number::numToStr()
{
	if (type_ == 1)
	{
		string s="";
		Rational* tmp = SCAST_RATIONAL(this);
		if (tmp->num_.sgn_) s+="-";
		s+=tmp->num_.number_;
		if (tmp->den_!=ONE_)
			s+="/" + tmp->den_.number_;
		return new String(s);
	}
	else if (type_ == 2)
	{
		stringstream ss;
		Float* tmp = SCAST_FLOAT(this);
		if ((fabs(tmp->number_)<=1e-4 || fabs(tmp->number_)>=1e20) && fabs(tmp->number_)>1e-300)
        	ss<<resetiosflags(ios::fixed)<<setprecision(30)<<tmp->number_;
    	else
        	ss<<fixed<<setprecision(30)<<tmp->number_;
        string str;
        ss>>str;
        return new String(str);
	}
	else if (type_ == 3)
	{
		bool real_no_zero=1;
		Complex* tmp = SCAST_COMPLEX(this);
		if (tmp->exact_)
   		{
        	string s;
        	Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
        	if (tmp_real->num_.sgn_) s+="-";
        	s+=tmp_real->num_.number_;
			if (tmp_real->den_!=ONE_)
				s+="/"+tmp_real->den_.number_;

        	Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
        	int FLAG=BigInt::abs_cmp(tmp_imag->num_.number_,"0");
        	if (FLAG==1 && !tmp_imag->num_.sgn_) s+="+";
        	else if (FLAG==0) { return new String(s); }
        	if (tmp_imag->num_.number_ == "1" && tmp_imag->den_.number_ == "1" )
        	{
            	if (!tmp_imag->num_.sgn_) { s+="i"; return new String(s);  }
            	else { s+="-i"; return new String(s);  }
        	}
        	if (tmp_imag->num_.sgn_) s+="-";
        	s+=tmp_imag->num_.number_;
			if (tmp_imag->den_!=ONE_)
				s+= "/"+tmp_imag->den_.number_;

			s+="i";
			return new String(s);
    	}
    	else
    	{
        	stringstream ss;

        	Float* tmp_real = SCAST_FLOAT(tmp->real_);
        	if ((fabs(tmp_real->number_)<=1e-4 || fabs(tmp_real->number_)>=1e20) && fabs(tmp_real->number_)>1e-300)
        		ss<<resetiosflags(ios::fixed)<<setprecision(30)<<tmp_real->number_;
    		else
        		ss<<fixed<<setprecision(30)<<tmp_real->number_;

        	Float* tmp_imag = SCAST_FLOAT(tmp->imag_);
        	if (tmp_imag->number_>=0)
            	ss<<"+";
        	if ((fabs(tmp_imag->number_)<=1e-4 || fabs(tmp_imag->number_)>=1e20) && fabs(tmp_imag->number_)>1e-300)
        		ss<<resetiosflags(ios::fixed)<<setprecision(30)<<tmp_imag->number_;
    		else
        		ss<<fixed<<setprecision(30)<<tmp_imag->number_;

        	ss<<"i";
        	string str; ss>>str; return new String(str);
    	}
	}
}

SchemeUnit* Number::numToStr(Number* number2)
{
	assert(number2->exact_ 
		&& "number->string second parameter is 2 8 10 or 16");
	int base;
	if (number2->type_ == 1)
	{
		Rational* tmp2 = SCAST_RATIONAL(number2);
		assert(tmp2->den_ == ONE_ 
			&& "number->string second parameter is 2 8 10 or 16");
		assert(!tmp2->num_.sgn_
			&& "number->string second parameter is 2 8 10 or 16");
		assert((   tmp2->num_.number_ == "2" 
				|| tmp2->num_.number_ == "8"
				|| tmp2->num_.number_ == "10"
				|| tmp2->num_.number_ == "16")
				&& "number->string second parameter is 2 8 10 or 16");
		base = (int)(double)tmp2->num_;
	}
	else if (number2->type_ == 3)
	{
		Complex* tmp2 = SCAST_COMPLEX(number2);
		Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
		Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
		assert(tmp2_imag->num_ == ZERO_
			&& "number->string second parameter is 2 8 10 or 16");
		assert(tmp2_real->den_ == ONE_ 
			&& "number->string second parameter is 2 8 10 or 16");
		assert(!tmp2_real->num_.sgn_
			&& "number->string second parameter is 2 8 10 or 16");
		assert((   tmp2_real->num_.number_ == "2" 
				|| tmp2_real->num_.number_ == "8"
				|| tmp2_real->num_.number_ == "10"
				|| tmp2_real->num_.number_ == "16")
				&& "number->string second parameter is 2 8 10 or 16");
		base = (int)(double)tmp2_real->num_;
	}

	if (type_ == 1)
	{
		BigInt B_base(base);

		Rational* tmp = SCAST_RATIONAL(this);
		string s="";
		if (tmp->num_.sgn_)
			s+="-";

		BigInt t_num = tmp->num_.abs();
		string s_num = "";
		while(t_num!=ZERO_)
		{
			BigInt remd = t_num % B_base;
			t_num = t_num / B_base;
			if (remd.number_ == "10") s_num+="a";
			else if (remd.number_ == "11") s_num+="b";
			else if (remd.number_ == "12") s_num+="c";
			else if (remd.number_ == "13") s_num+="d";
			else if (remd.number_ == "14") s_num+="e";
			else if (remd.number_ == "15") s_num+="f";
			else s_num+=remd.number_;
		}
		reverse(s_num.begin(),s_num.end());

		if (tmp->den_==ONE_) return new String(s+s_num);

		BigInt t_den = tmp->den_.abs();
		string s_den = "";
		while(t_den!=ZERO_)
		{
			BigInt remd = t_den % B_base;
			t_den = t_den / B_base;
			if (remd.number_ == "10") s_den += "a";
			else if (remd.number_ == "11") s_den += "b";
			else if (remd.number_ == "12") s_den += "c";
			else if (remd.number_ == "13") s_den += "d";
			else if (remd.number_ == "14") s_den += "e";
			else if (remd.number_ == "15") s_den += "f";
			else s_den += remd.number_;
		}
		reverse(s_den.begin(),s_den.end());

		return new String(s+s_num+"/"+s_den);
	}
	else if (type_ == 2)
	{
		stringstream ss;
		assert(base == 10 && "inexact numbers can be only printed in base 10");
		Float* tmp = SCAST_FLOAT(this);
		if ((fabs(tmp->number_)<=1e-4 || fabs(tmp->number_)>=1e20) && fabs(tmp->number_)>1e-300)
        	ss<<resetiosflags(ios::fixed)<<setprecision(30)<<tmp->number_;
    	else
        	ss<<fixed<<setprecision(30)<<tmp->number_;
        string str;
        ss>>str;
        return new String(str);
	}
	else if (type_ == 3)
	{
		bool real_no_zero=1;
		Complex* tmp = SCAST_COMPLEX(this);
		if (tmp->exact_)
   		{
        	BigInt B_base(base);

        	//real part
        	Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
			string s_r ="";
			if (tmp_real->num_.sgn_)
				s_r+="-";
			//real part num
			BigInt t_r_num = tmp_real->num_.abs();
			string s_r_num = "";
			while(t_r_num!=ZERO_)
			{
				BigInt remd = t_r_num % B_base;
				t_r_num = t_r_num / B_base;
				if (remd.number_ == "10") s_r_num+="a";
				else if (remd.number_ == "11") s_r_num+="b";
				else if (remd.number_ == "12") s_r_num+="c";
				else if (remd.number_ == "13") s_r_num+="d";
				else if (remd.number_ == "14") s_r_num+="e";
				else if (remd.number_ == "15") s_r_num+="f";
				else s_r_num+=remd.number_;
			}
			reverse(s_r_num.begin(),s_r_num.end());

			//real part den
			BigInt t_r_den = tmp_real->den_.abs();
			string s_r_den = "";
			if (tmp_real->den_!=ONE_)
			{
				while(t_r_den!=ZERO_)
				{
					BigInt remd = t_r_den % B_base;
					t_r_den = t_r_den / B_base;
					if (remd.number_ == "10") s_r_den += "a";
					else if (remd.number_ == "11") s_r_den += "b";
					else if (remd.number_ == "12") s_r_den += "c";
					else if (remd.number_ == "13") s_r_den += "d";
					else if (remd.number_ == "14") s_r_den += "e";
					else if (remd.number_ == "15") s_r_den += "f";
					else s_r_den += remd.number_;
				}
				reverse(s_r_den.begin(),s_r_den.end());
				s_r += s_r_num + "/" + s_r_den;
			}
			else s_r += s_r_num;

			//imag part
			string s_i = "";
        	Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
        	int FLAG=BigInt::abs_cmp(tmp_imag->num_.number_,"0");
        	if (FLAG==1 && !tmp_imag->num_.sgn_) s_i+="+";
        	else if (FLAG==0) { return new String(s_r+s_i); }
        	if (tmp_imag->num_.number_ == "1" && tmp_imag->den_.number_ == "1" )
        	{
            	if (!tmp_imag->num_.sgn_) { s_i+="i"; return new String(s_r+s_i);  }
            	else {s_i+="-i"; return new String(s_r+s_i);  }
        	}
        	if (tmp_imag->num_.sgn_)
				s_i+="-";

        	//imag part num
        	BigInt t_i_num = tmp_imag->num_.abs();
			string s_i_num = "";
			while(t_i_num!=ZERO_)
			{
				BigInt remd = t_i_num % B_base;
				t_i_num = t_i_num / B_base;
				if (remd.number_ == "10") s_i_num+="a";
				else if (remd.number_ == "11") s_i_num+="b";
				else if (remd.number_ == "12") s_i_num+="c";
				else if (remd.number_ == "13") s_i_num+="d";
				else if (remd.number_ == "14") s_i_num+="e";
				else if (remd.number_ == "15") s_i_num+="f";
				else s_i_num+=remd.number_;
			}
			reverse(s_i_num.begin(),s_i_num.end());

			//imag part den
			BigInt t_i_den = tmp_imag->den_.abs();
			string s_i_den = "";
			if (tmp_imag->den_!=ONE_)
			{
				while(t_i_den!=ZERO_)
				{
					BigInt remd = t_i_den % B_base;
					t_i_den = t_i_den / B_base;
					if (remd.number_ == "10") s_i_den += "a";
					else if (remd.number_ == "11") s_i_den += "b";
					else if (remd.number_ == "12") s_i_den += "c";
					else if (remd.number_ == "13") s_i_den += "d";
					else if (remd.number_ == "14") s_i_den += "e";
					else if (remd.number_ == "15") s_i_den += "f";
					else s_i_den += remd.number_;
				}
				reverse(s_i_den.begin(),s_i_den.end());
				s_i += s_i_num + "/" + s_i_den;
			}
			else s_i += s_i_num;

			s_i+="i";
			return new String(s_r+s_i);
    	}
    	else
    	{
        	stringstream ss;
        	assert(base==10 && "inexact numbers can only be printed in base 10");
        	Float* tmp_real = SCAST_FLOAT(tmp->real_);
        	if ((fabs(tmp_real->number_)<=1e-4 || fabs(tmp_real->number_)>=1e20) && fabs(tmp_real->number_)>1e-300)
        		ss<<resetiosflags(ios::fixed)<<setprecision(30)<<tmp_real->number_;
    		else
        		ss<<fixed<<setprecision(30)<<tmp_real->number_;

        	Float* tmp_imag = SCAST_FLOAT(tmp->imag_);
        	if (tmp_imag->number_>=0)
            	ss<<"+";
        	if ((fabs(tmp_imag->number_)<=1e-4 || fabs(tmp_imag->number_)>=1e20) && fabs(tmp_imag->number_)>1e-300)
        		ss<<resetiosflags(ios::fixed)<<setprecision(30)<<tmp_imag->number_;
    		else
        		ss<<fixed<<setprecision(30)<<tmp_imag->number_;

        	ss<<"i";
        	string str; ss>>str; return new String(str);
    	}
	}
}
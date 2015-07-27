#include "mystring.h"
#include "boolean.h"
#include "bigint.h"
#include "complex.h"
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

String::String(string str):str_(str)
{
	unitType_ = STRING;
}

String::~String()
{

}

void String::print()
{
	cout<<"\""<<str_<<"\"";
}

String* String::from_string(char* expression)
{
	int len = strlen(expression);
	if (len<2) return NULL;
	else if (expression[0]!='\"' || expression[len-1]!='\"')
		return NULL;
	else if (expression[0]=='\"' && expression[len-1]=='\"')
	{
		if (len == 3 && expression[len-2]=='\\') return NULL;
		else if (len>=4 && expression[len-2]=='\\')
		{
			if (expression[len-3]!='\\') return NULL;
		}
	}
	string s = "";
	for (int i=1;i<len-1;++i)
	{
		if (expression[i]!='\\') s += expression[i];
		else s += expression[++i];
	}
	return new String(s);
}

SchemeUnit* String::nott()
{
	return new Boolean(false);
}

SchemeUnit* String::isInteger()
{
	return new Boolean(false);
}

SchemeUnit* String::isRational()
{
	return new Boolean(false);
}

SchemeUnit* String::isReal()
{
	return new Boolean(false);
}

SchemeUnit* String::isComplex()
{
	return new Boolean(false);
}

SchemeUnit* String::isNumber()
{
	return new Boolean(false);
}

SchemeUnit* String::isChar()
{
	return new Boolean(false);
}

SchemeUnit* String::isString()
{
	return new Boolean(true);
}

String* String::makeString(Number* number, Character* c)
{
	assert(number->type_!=2 
		&& "make-string parameter is non-negative integer");
	if (number->type_ == 1)
	{
		Rational* tmp = SCAST_RATIONAL(number);
		assert(tmp->den_==ONE_ 
			&& "make-string parameter is non-negative integer");
		assert(!tmp->num_.sgn_ 
			&& "make-string parameter is non-negative integer");
		string s="";
		int i_num = (int)(double)(tmp->num_);
		for (int i=1; i <= i_num; ++i)
		{
			s+=c->ch_;
		}
		return new String(s);
	}
	else if (number->type_ == 3)
	{
		Complex* tmp = SCAST_COMPLEX(number);
		assert(tmp->exact_ 
			&& "make-string parameter is non-negative integer");
		Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
		Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
		assert(tmp_imag->num_==ZERO_
			&& "make-string parameter is non-negative integer");
		assert(tmp_real->den_==ONE_
			&& "make-string parameter is non-negative integer");
		assert(!tmp_real->num_.sgn_
			&& "make-string parameter is non-negative integer");
		string s="";
		int i_num = (int)(double)(tmp_real->num_);
		for (int i=1; i <= i_num; ++i)
		{
			s+=c->ch_;
		}
		return new String(s);
	}
	
}

String* String::formString(Character* c)
{
	return new String(str_+=c->ch_);
}

SchemeUnit* String::strLen()
{
	return new Rational(BigInt(str_.length()),ONE_);
}

SchemeUnit* String::strRef(Number* number)
{
	assert(number->type_!=2 
		&& "make-string parameter is non-negative integer");
	if (number->type_ == 1)
	{
		Rational* tmp = SCAST_RATIONAL(number);
		assert(tmp->den_==ONE_ 
			&& "make-string parameter is non-negative integer");
		assert(!tmp->num_.sgn_ 
			&& "make-string parameter is non-negative integer");
		int i_num = (int)(double)(tmp->num_);
		assert(i_num<str_.length()
			&& "given index out of valid range");
		return new Character(str_[i_num]);
	}
	else if (number->type_ == 3)
	{
		Complex* tmp = SCAST_COMPLEX(number);
		assert(tmp->exact_ 
			&& "make-string parameter is non-negative integer");
		Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
		Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
		assert(tmp_imag->num_==ZERO_
			&& "make-string parameter is non-negative integer");
		assert(tmp_real->den_==ONE_
			&& "make-string parameter is non-negative integer");
		assert(!tmp_real->num_.sgn_
			&& "make-string parameter is non-negative integer");
		int i_num = (int)(double)(tmp_real->num_);
		assert(i_num<str_.length()
			&& "given index out of valid range");
		return new Character(str_[i_num]);
	}
}

SchemeUnit* String::strEql(String* str2)
{
	return new Boolean(str_.compare(str2->str_) == 0);
}

SchemeUnit* String::strMoInc(String* str2)
{
	return new Boolean(str_.compare(str2->str_) == -1);
}

SchemeUnit* String::strNonDec(String* str2)
{
	int i_res = str_.compare(str2->str_);
	return new Boolean(i_res == 0 || i_res == -1 );
}

SchemeUnit* String::strMoDec(String* str2)
{
	return new Boolean(str_.compare(str2->str_) == 1);
}

SchemeUnit* String::strNonInc(String* str2)
{
	int i_res = str_.compare(str2->str_);
	return new Boolean(i_res == 0 || i_res == 1);
}

SchemeUnit* String::strCiEql(String* str2)
{
	string tmp1="",tmp2="";
	for (int i=0;i<str_.length();++i)
	{
		if (str_[i]>='a' && str_[i]<='z')
			tmp1+=(char)(str_[i]-32);
		else tmp1+=str_[i];
	}
	for (int i=0;i<str2->str_.length();++i)
	{
		if (str2->str_[i]>='a' && str2->str_[i]<='z')
			tmp2+=(char)(str2->str_[i]-32);
		else tmp2+=str2->str_[i];
	}
	return new Boolean(tmp1.compare(tmp2)==0);
}

SchemeUnit* String::strCiMoInc(String* str2)
{
	string tmp1="",tmp2="";
	for (int i=0;i<str_.length();++i)
	{
		if (str_[i]>='a' && str_[i]<='z')
			tmp1+=(char)(str_[i]-32);
		else tmp1+=str_[i];
	}
	for (int i=0;i<str2->str_.length();++i)
	{
		if (str2->str_[i]>='a' && str2->str_[i]<='z')
			tmp2+=(char)(str2->str_[i]-32);
		else tmp2+=str2->str_[i];
	}
	return new Boolean(tmp1.compare(tmp2) == -1);
}

SchemeUnit* String::strCiNonDec(String* str2)
{
	string tmp1="",tmp2="";
	for (int i=0;i<str_.length();++i)
	{
		if (str_[i]>='a' && str_[i]<='z')
			tmp1+=(char)(str_[i]-32);
		else tmp1+=str_[i];
	}
	for (int i=0;i<str2->str_.length();++i)
	{
		if (str2->str_[i]>='a' && str2->str_[i]<='z')
			tmp2+=(char)(str2->str_[i]-32);
		else tmp2+=str2->str_[i];
	}
	int i_res = tmp1.compare(tmp2);
	return new Boolean(i_res == 0 || i_res == -1);
}

SchemeUnit* String::strCiMoDec(String* str2)
{
	string tmp1="",tmp2="";
	for (int i=0;i<str_.length();++i)
	{
		if (str_[i]>='a' && str_[i]<='z')
			tmp1+=(char)(str_[i]-32);
		else tmp1+=str_[i];
	}
	for (int i=0;i<str2->str_.length();++i)
	{
		if (str2->str_[i]>='a' && str2->str_[i]<='z')
			tmp2+=(char)(str2->str_[i]-32);
		else tmp2+=str2->str_[i];
	}
	return new Boolean(tmp1.compare(tmp2) == 1);
}

SchemeUnit* String::strCiNonInc(String* str2)
{
	string tmp1="",tmp2="";
	for (int i=0;i<str_.length();++i)
	{
		if (str_[i]>='a' && str_[i]<='z')
			tmp1+=(char)(str_[i]-32);
		else tmp1+=str_[i];
	}
	for (int i=0;i<str2->str_.length();++i)
	{
		if (str2->str_[i]>='a' && str2->str_[i]<='z')
			tmp2+=(char)(str2->str_[i]-32);
		else tmp2+=str2->str_[i];
	}
	int i_res = tmp1.compare(tmp2);
	return new Boolean(i_res == 0 || i_res == 1);
}

SchemeUnit* String::subStr(Number* number)
{
	assert(number->type_!=2 
		&& "substring parameter is non-negative integer");
	if (number->type_ == 1)
	{
		Rational* tmp = SCAST_RATIONAL(number);
		assert(tmp->den_ == ONE_ 
			&& "substring parameter is non-negative integer");
		assert(!tmp->num_.sgn_ 
			&& "substring parameter is non-negative integer");
		int i_num = (int)(double)(tmp->num_);
		int len = str_.length();
		assert(i_num<len 
			&& "given index out of valid range");
		return new String(str_.substr(i_num,len-i_num));
	}
	else if (number->type_ == 3)
	{
		Complex* tmp = SCAST_COMPLEX(number);
		assert(tmp->exact_ 
			&& "substring parameter is non-negative integer");
		Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
		Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
		assert(tmp_imag->num_==ZERO_
			&& "substring parameter is non-negative integer");
		assert(tmp_real->den_==ONE_
			&& "substring parameter is non-negative integer");
		assert(!tmp_real->num_.sgn_
			&& "substring parameter is non-negative integer");
		int i_num = (int)(double)(tmp_real->num_);
		int len = str_.length();
		assert(i_num<len 
			&& "given index out of valid range");
		return new String(str_.substr(i_num,len-i_num));
	}
}

SchemeUnit* String::subStr(Number* number1, Number* number2)
{
	assert(number1->type_!=2 && number2->type_!=2
		&& "substring parameter is non-negative integer");
	int i_bg,i_ed;
	int len = str_.length();
	if (number1->type_ == 1)
	{
		Rational* tmp = SCAST_RATIONAL(number1);
		assert(tmp->den_ == ONE_ 
			&& "substring parameter is non-negative integer");
		assert(!tmp->num_.sgn_ 
			&& "substring parameter is non-negative integer");
		i_bg = (int)(double)(tmp->num_);
		assert(i_bg<len 
			&& "given index out of valid range");
	}
	else if (number1->type_ == 3)
	{
		Complex* tmp = SCAST_COMPLEX(number1);
		assert(tmp->exact_ 
			&& "substring parameter is non-negative integer");
		Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
		Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
		assert(tmp_imag->num_==ZERO_
			&& "substring parameter is non-negative integer");
		assert(tmp_real->den_==ONE_
			&& "substring parameter is non-negative integer");
		assert(!tmp_real->num_.sgn_
			&& "substring parameter is non-negative integer");
		i_bg = (int)(double)(tmp_real->num_);
		assert(i_bg<len 
			&& "given index out of valid range");
	}

	if (number2->type_ == 1)
	{
		Rational* tmp = SCAST_RATIONAL(number2);
		assert(tmp->den_ == ONE_ 
			&& "substring parameter is non-negative integer");
		assert(!tmp->num_.sgn_ 
			&& "substring parameter is non-negative integer");
		i_ed = (int)(double)(tmp->num_);
		assert(i_ed<len 
			&& "given index out of valid range");
	}
	else if (number2->type_ == 3)
	{
		Complex* tmp = SCAST_COMPLEX(number2);
		assert(tmp->exact_ 
			&& "substring parameter is non-negative integer");
		Rational* tmp_real = SCAST_RATIONAL(tmp->real_);
		Rational* tmp_imag = SCAST_RATIONAL(tmp->imag_);
		assert(tmp_imag->num_==ZERO_
			&& "substring parameter is non-negative integer");
		assert(tmp_real->den_==ONE_
			&& "substring parameter is non-negative integer");
		assert(!tmp_real->num_.sgn_
			&& "substring parameter is non-negative integer");
		i_ed = (int)(double)(tmp_real->num_);
		assert(i_ed<len 
			&& "given index out of valid range");
	}

	assert(i_bg<=i_ed 
		&& "ending index is smaller than starting index");
	return new String(str_.substr(i_bg,i_ed-i_bg));
}

String* String::strApd(String* str2)
{
	return new String(str_+str2->str_);
}

SchemeUnit* String::strCpy()
{
	return new String(str_);
}

SchemeUnit* String::strToNum()
{
	int len = str_.length();
	char* cstr = new char [len+2];
	for (int i=0;i<len;++i)
		cstr[i]=str_[i];
	cstr[len]='\0';
	Number* res;
	res = Rational::from_string(cstr);
	if (!res) res = Float::from_string(cstr);
	if (!res) res = Complex::from_string(cstr);

	if (!res) { delete cstr; return new Boolean(false); }
	else {delete cstr; return res; }
}

SchemeUnit* String::strToNum(Number* number2)
{
	assert(number2->exact_ 
		&& "string->number second parameter is 2 8 10 or 16");
	int base;
	if (number2->type_ == 1)
	{
		Rational* tmp2 = SCAST_RATIONAL(number2);
		assert(tmp2->den_ == ONE_ 
			&& "string->number second parameter is 2 8 10 or 16");
		assert(!tmp2->num_.sgn_
			&& "string->number second parameter is 2 8 10 or 16");
		assert((   tmp2->num_.number_ == "2" 
				|| tmp2->num_.number_ == "8"
				|| tmp2->num_.number_ == "10"
				|| tmp2->num_.number_ == "16")
				&& "string->number second parameter is 2 8 10 or 16");
		base = (int)(double)tmp2->num_;
	}
	else if (number2->type_ == 3)
	{
		Complex* tmp2 = SCAST_COMPLEX(number2);
		Rational* tmp2_real = SCAST_RATIONAL(tmp2->real_);
		Rational* tmp2_imag = SCAST_RATIONAL(tmp2->imag_);
		assert(tmp2_imag->num_ == ZERO_
			&& "string->number second parameter is 2 8 10 or 16");
		assert(tmp2_real->den_ == ONE_ 
			&& "string->number second parameter is 2 8 10 or 16");
		assert(!tmp2_real->num_.sgn_
			&& "string->number second parameter is 2 8 10 or 16");
		assert((   tmp2_real->num_.number_ == "2" 
				|| tmp2_real->num_.number_ == "8"
				|| tmp2_real->num_.number_ == "10"
				|| tmp2_real->num_.number_ == "16")
				&& "string->number second parameter is 2 8 10 or 16");
		base = (int)(double)tmp2_real->num_;
	}

	if (base==10)
	{
		int len = str_.length();
		char* cstr = new char [len+2];
		for (int i=0;i<len;++i)
			cstr[i]=str_[i];
		cstr[len]='\0';
		Number* res;
		res = Rational::from_string(cstr);
		if (!res) res = Float::from_string(cstr);
		if (!res) res = Complex::from_string(cstr);

		if (!res) { delete cstr; return new Boolean(false); }
		else {delete cstr; return res; }
	}
	else return NULL;
	
	/*int len = str_.length();
	char* cstr = new char [len+2];
	for (int i=0;i<len;++i)
		cstr[i]=str_[i];
	cstr[len]='\0';
	BigInt B_base(base);
	if (base == 2)
	{
		for (int i=0;i<len;++i)
		{
			if (str_[i]=='.' || str_[i]=='i' || str_[i]=='/' || str_[i]=='+' 
				|| str_[i]=='-' || str_[i]=='e' || str_[i]=='E')
				continue;
			if (str_[i]>='2')
				return new Boolean(false);
		}

		if (str_.find("i")!=-1) //complex
		{
			Complex* res = new Complex;
			Number* pre_real = res->real_;
			Number* pre_imag = res->imag_;
			Number* real,imag;
			bool isExact=true;

			string real_str, imag_str;
    		if (str_[len-1]!='i') return NULL;
    		if (len==1 && str_[0]=='i') { real_str = "0"; imag_str = "1"; }
    		else if (len==2 && str_[0]=='-' && str_[1]=='i') { real_str = "0"; imag_str = "-1";}
    		else
    		{
        		int imag_part_begin = str_.length()-1;
        		for (; imag_part_begin>=0; --imag_part_begin)
            		if (str_[imag_part_begin] == '+' || str_[imag_part_begin] == '-')
                		if (imag_part_begin==0 || (str_[imag_part_begin-1]!='e' && str_[imag_part_begin-1]!='E'))
                    		break;
        		if (imag_part_begin==-1) imag_part_begin=0;
        		real_str = (imag_part_begin==0) ? "0" : str_.substr(0,imag_part_begin);
        		imag_str = str_.substr(imag_part_begin, len - imag_part_begin-1);
        		if (imag_str == "+") imag_str="1";
        		else if (imag_str == "-") imag_str="-1";
    		}
    		//real part
    		if (real_str.find("i")!=-1) //real part complex
    			return new Boolean(false);
    		else if (real_str.find(".")!=-1 || real_str.find("e") || real_str.find("E") ) //real part float
    		{
    			isExact = false;
    			if (real_str.find(".")!=-1) // dot
    			{
    				if (real_str.find("e")!=-1 || real_str.find("E")!=-1)
    					return new Boolean (false);
    				int dot_pos = real_str.find(".");
    				string no_dot = real_str;
    				no_dot.erase(dot_pos,1);
    				bool MINUS=0;
    				if (no_dot[0]=='-')
    				{
    					MINUS=1;
    					no_dot.erase(0,1);
    				}
    				else if (no_dot[0]=='+') no_dot.erase(0,1);

    				BigInt t_bs=ONE_;
    				BigInt num=ZERO_;
    				for (int i=no_dot.length()-1;i>=0;--i)
    				{
    					string digit = "";
    					digit+=no_dot[i];
    					num = num + t_bs * BigInt(digit);
    					t_bs = t_bs * B_base;
    				}
    				BigInt den=ONE_;
    				for (int i=dot_pos+1;i<len;++i)
    					den = den * B_base;

    				double val = (double)num/(double)den;
    				if (MINUS) val = 0.0-val;
    				real = new Float(val);
    			}
    			else // e or E
    			{
    				int e_pos = real_str.find("e");
    				if (e_pos == -1)
    					e_pos = real_str.find("E");
    				BigInt num(real_str.substr(0,e_pos));
    			}
    		}
    		else //real part rational
    		{
    			if (real_str.find("/")) // has /
    			{
    				int sp_pos = real_str.find("/");
    				BigInt num(real_str.substr(0,sp_pos));
    				BigInt den(real_str.substr(sp_pos+1,real_str.length()-sp_pos-1));
    				real = new Rational(num,den);
    			}
    			else// no /
    			{
    				BigInt num(real_str);
    				real = new Rational(num,ONE_);
    			}
    		}

    		
    		
		}
		else if (str_.find(".")!=-1 || str_.find("e")!=-1 || str_.find("E")!=-1 ) //float
		{

		}
		else //rational
		{

		}
	}

	else if (base == 8)
	{
		for (int i=0;i<len;++i)
		{
			if (str_[i]=='.' || str_[i]=='i' || str_[i]=='/' || str_[i]=='+' 
				|| str_[i]=='-' || str_[i]=='e' || str_[i]=='E')
				continue;
			if (str_[i]>='8')
				return new Boolean(false);
		}
			
	}

	else if (base == 10)
	{
		for (int i=0;i<len;++i)
		{
			if (str_[i]=='.' || str_[i]=='i' || str_[i]=='/' || str_[i]=='+' 
				|| str_[i]=='-' || str_[i]=='e' || str_[i]=='E')
				continue;
			if (str_[i]>'9')
				return new Boolean(false);
		}
			
	}

	else if (base == 16)
	{
		for (int i=0;i<len;++i)
		{
			if (str_[i]=='.' || str_[i]=='i' || str_[i]=='/' || str_[i]=='+' 
				|| str_[i]=='-' || str_[i]=='e' || str_[i]=='E')
				continue;
			if (str_[i]>'9' 
				&& !(str_[i]>='A' && str_[i]<='F') 
				&& !(str_[i]>='a' && str_[i]<='f') )
					return new Boolean(false);
		}
			
	}*/

}
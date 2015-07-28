#ifndef mystring_h
#define mystring_h

#include "schemeunit.h"
#include "rational.h"
#include "character.h"
#define SCAST_STRING(x) static_cast<String*>(x)

class String:public SchemeUnit{
public:
	string str_;
	String(string str ="");
	~String();

	virtual SchemeUnit* nott();
    virtual SchemeUnit* isInteger();
    virtual SchemeUnit* isRational();
    virtual SchemeUnit* isReal();
    virtual SchemeUnit* isComplex();
    virtual SchemeUnit* isNumber();
    virtual SchemeUnit* isChar();
    virtual SchemeUnit* isString();
    virtual SchemeUnit* strLen();
    virtual SchemeUnit* strRef(Number* number);
    virtual SchemeUnit* strEql(String* str2);
    virtual SchemeUnit* strMoInc(String* str2);
    virtual SchemeUnit* strNonDec(String* str2);
    virtual SchemeUnit* strMoDec(String* str2);
    virtual SchemeUnit* strNonInc(String* str2);
    virtual SchemeUnit* strCiEql(String* str2);
    virtual SchemeUnit* strCiMoInc(String* str2);
    virtual SchemeUnit* strCiNonDec(String* str2);
    virtual SchemeUnit* strCiMoDec(String* str2);
    virtual SchemeUnit* strCiNonInc(String* str2);
    virtual SchemeUnit* subStr(Number* number);
    virtual SchemeUnit* subStr(Number* number1, Number* number2);
    virtual SchemeUnit* strCpy();
    virtual SchemeUnit* strToNum();
    virtual SchemeUnit* strToNum(Number* number2);
    virtual SchemeUnit* isEql(String* str2);
    virtual String* strApd(String* str2);
    virtual String* formString(Character* c);

    virtual void print();

    static String* from_string(char* expression);
    static String* makeString(Number* number, Character* c);

};


#endif // mystring_h
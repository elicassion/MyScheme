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
    virtual String* formString(Character* c);

    virtual void print();

    static String* from_string(char* expression);
    static String* makeString(Number* number, Character* c);

};


#endif // mystring_h
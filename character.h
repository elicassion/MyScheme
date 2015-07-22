#ifndef character_h
#define character_h

#include "schemeunit.h"
#define SCAST_CHARACTER(x) static_cast<Character*>(x)
class Character:public SchemeUnit{
public:
    char ch_;
    Character(char ch='\0');
    ~Character();

    char to_upper_case(char ch);
    char to_lower_case(char ch);

    virtual SchemeUnit* nott();
    virtual SchemeUnit* isInteger();
    virtual SchemeUnit* isRational();
    virtual SchemeUnit* isReal();
    virtual SchemeUnit* isComplex();
    virtual SchemeUnit* isNumber();
    virtual SchemeUnit* isChar();
    virtual SchemeUnit* isString();
    virtual SchemeUnit* chEql(Character* ch2);
    virtual SchemeUnit* chMoInc(Character* ch2);
    virtual SchemeUnit* chNonDec(Character* ch2);
    virtual SchemeUnit* chMoDec(Character* ch2);
    virtual SchemeUnit* chNonInc(Character* ch2);
    virtual SchemeUnit* chCiEql(Character* ch2);
    virtual SchemeUnit* chCiMoInc(Character* ch2);
    virtual SchemeUnit* chCiNonDec(Character* ch2);
    virtual SchemeUnit* chCiMoDec(Character* ch2);
    virtual SchemeUnit* chCiNonInc(Character* ch2);
    virtual SchemeUnit* chIsAlpha();
    virtual SchemeUnit* chIsNum();
    virtual SchemeUnit* chIsSpace();
    virtual SchemeUnit* chIsUpCase();
    virtual SchemeUnit* chIsLowCase();
    virtual SchemeUnit* chToInt();
    virtual SchemeUnit* chToUp();
    virtual SchemeUnit* chToDown();
    virtual void print();
    static Character* from_string(char* expression);

};


#endif // charater_h

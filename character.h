#ifndef character_h
#define character_h

#include "schemeunit.h"

class Character:public SchemeUnit{
public:
    char ch_;
    Character(char ch='\0');
    ~Character();

    virtual SchemeUnit* nott();
    virtual SchemeUnit* isInteger();
    virtual SchemeUnit* isRational();
    virtual SchemeUnit* isReal();
    virtual SchemeUnit* isComplex();
    virtual SchemeUnit* isNumber();
    virtual void print();
    static Character* from_string(char* expression);

};
#endif // charater_h

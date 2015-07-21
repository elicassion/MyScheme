#ifndef boolean_h
#define boolean_h

#include "schemeunit.h"
#define SCAST_BOOLEAN static_cast<Boolean*>(x)

class Boolean:public SchemeUnit{
public:
    bool value_;
    Boolean(bool value = false);
    ~Boolean();
    virtual SchemeUnit* nott();
    virtual SchemeUnit* isInteger();
    virtual SchemeUnit* isRational();
    virtual SchemeUnit* isReal();
    virtual SchemeUnit* isComplex();
    virtual SchemeUnit* isNumber();
    virtual void print();
};

#endif // boolean_h

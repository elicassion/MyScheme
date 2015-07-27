#include "boolean.h"
#include <iostream>
using namespace std;
Boolean::Boolean(bool value):value_(value)
{
    unitType_=BOOLEAN;
}

Boolean::~Boolean() {}

SchemeUnit* Boolean::nott()
{
    return new Boolean(!value_);
}

SchemeUnit* Boolean::boolAnd(SchemeUnit* b2)
{
	Boolean* tmp = SCAST_BOOLEAN(b2);
	return new Boolean(value_ && tmp->value_);
}

void Boolean::print()
{
    if (value_) cout<<"#t";
    else cout<<"#f";
}

SchemeUnit* Boolean::isInteger() { return new Boolean(false); }

SchemeUnit* Boolean::isRational() { return new Boolean(false); }

SchemeUnit* Boolean::isReal() { return new Boolean(false); }

SchemeUnit* Boolean::isComplex() { return new Boolean(false); }

SchemeUnit* Boolean::isNumber() { return new Boolean(false); }

SchemeUnit* Boolean::isChar() { return new Boolean(false); }

SchemeUnit* Boolean::isString() { return new Boolean(false); }

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

void Boolean::print()
{
    if (value_) cout<<"#t";
    else cout<<"#f";
}

#include "number.h"
#include "boolean.h"
Number::Number()
{
    unitType_ = NUMBER;
}

SchemeUnit* Number::nott()
{
    return new Boolean(false);
}

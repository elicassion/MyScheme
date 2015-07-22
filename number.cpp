#include "number.h"
#include "boolean.h"
#include "character.h"
#include "rational.h"
#include "bigint.h"
#include <cassert>
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
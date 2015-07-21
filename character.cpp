#include "character.h"
#include "boolean.h"
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
Character::Character(char ch):ch_(ch)
{
    unitType_ = CHARACTER;
}

Character::~Character()
{

}

Character* Character::from_string(char* expression)
{
    if (strlen(expression)!=3) return NULL; //to be continued
    if (!(expression[0]=='#' && expression[1]=='\\'))
        return NULL;
    return new Character(expression[2]);
}

void Character::print()
{
    printf("%c",ch_);
}

SchemeUnit* Character::nott()
{
    return new Boolean(false);
}

SchemeUnit* Character::isInteger() { return new Boolean(false); }

SchemeUnit* Character::isRational() { return new Boolean(false); }

SchemeUnit* Character::isReal() { return new Boolean(false); }

SchemeUnit* Character::isComplex() { return new Boolean(false); }

SchemeUnit* Character::isNumber() { return new Boolean(false); }

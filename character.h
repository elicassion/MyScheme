#ifndef character_h
#define character_h

#include "schemeunit.h"

class Character:public SchemeUnit{
public:
    char ch_;
    Character(char ch='\0');
    ~Character();

    virtual SchemeUnit* nott();
    virtual void print();
    static Character* from_string(char* expression);

};
#endif // charater_h

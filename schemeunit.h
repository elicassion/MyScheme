#ifndef schemeunit_h
#define schemeunit_h

class SchemeUnit{
public:
    enum{
        BOOLEAN=1,
        NUMBER=2,
        CHAR=3,
        STRING=4
    } unitType_;
    virtual void print() = 0;
};

#endif // schemeunit_h

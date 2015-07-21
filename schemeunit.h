#ifndef schemeunit_h
#define schemeunit_h
class SchemeUnit{
public:
    enum{
        BOOLEAN=1,
        NUMBER=2,
        CHARACTER=3,
        STRING=4
    } unitType_;

    SchemeUnit() {};
    virtual ~SchemeUnit() {};
    virtual SchemeUnit* nott() =0;
    virtual SchemeUnit* isInteger() =0;
    virtual SchemeUnit* isRational() =0;
    virtual SchemeUnit* isReal() =0;
    virtual SchemeUnit* isComplex() =0;
    virtual SchemeUnit* isNumber() =0;
    virtual void print() =0;
};

#endif // schemeunit_h

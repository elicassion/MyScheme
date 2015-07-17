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
    virtual void print() =0;
};

#endif // schemeunit_h

#ifndef opt_h
#define opt_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <bitset>
#include <cctype>
#include <cassert>
#include <complex>
#include "number.h"
#include "schemeunit.h"

struct Cons {
    SchemeUnit *car;
    Cons *cdr;
    Cons(SchemeUnit *_car, Cons *_cdr) : car(_car), cdr(_cdr) {}
    bool check_length(int len) {
        int l = 0;
        for (Cons *p = this; p; p = p->cdr) l++;
        return l == len;
    }
};

class Opt {
    public:
        virtual SchemeUnit *calc(Cons *con) = 0;
};


#endif // opt_h

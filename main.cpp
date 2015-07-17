#include "calc_exp.h"
#include <iostream>
using namespace std;

int main() {
    try {
		for (SchemeUnit *res;;) {
			res = calc_exp();
			res -> print();
			printf("\n");
		}
	}
	catch(int){cerr<<"Wrong";}
	//system("pause");
    return 0;
}

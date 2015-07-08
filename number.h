#ifndef number_h
#define number_h

#define SCAST_NUMBER(x) static_cast<Number*>(x)

class Number {
public:
	enum{
		RATIONAL = 1,
		FLOAT = 2
	} type_;
	Number(){}
	virtual ~Number(){}
	virtual Number *convert(Number *number2) = 0;
	virtual Number *add(Number *number2)  = 0;
	virtual Number *sub(Number *number2)  = 0;
	virtual Number *mul(Number *number2)  = 0;
	virtual Number *div(Number *number2)  = 0;
	virtual void print() = 0;
};


#endif // number_h

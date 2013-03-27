#ifndef OPERATOR_H_
#define OPERATOR_H_

#include "opSymb.h"

class Operator
{
	double result;		//Result of the function on the results of the two operands
	Operator *lOperand;	//Pointer to the left operand
	Operator *rOperand;	//Pointer to the right operand
	char *opLoc;		//Pointer to the character in the expression string of the operator
	OPFUNC *opFunc;		//Pointer to the function represented by the symbol pointed to by opLoc

public:
	Operator(char *op, Operator * const lop, Operator * const rop);

public:
	void setLeft(Operator * const oper);
	void setRight(Operator * const oper);
	const bool setLoc(char * const loc);
	static const bool isValidNum(char c);	//returns true if the character is a valid number character (numeral, '-', '.')

	const Operator * const getLeft() const;
	const Operator * const getRight() const;
	char * const getLoc() const;
	const double getResult() throw(string);	//returns the result of the Operator, will call getResult of Operators if they aren't NULL.
											//Throws exception if a function of one instance is called twice.
private:
	bool curCalc;	//used to regulate how many times a single instance of getResult is called
};



#endif /*OPERATOR_H_*/
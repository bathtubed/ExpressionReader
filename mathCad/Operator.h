#ifndef OPERATOR_H_
#define OPERATOR_H_

#include "opSymb.h"

class Operator
{
public:
	typedef enum {INVALID, NUM, SYM, VAR} op_t;

	double result;		//Result of the function on the results of the two operands
	Operator *lOperand;	//Pointer to the left operand
	Operator *rOperand;	//Pointer to the right operand
	char *opLoc;		//Pointer to the character in the expression string of the operator
	opSymb *opFunc;		//Pointer to the function represented by the symbol pointed to by opLoc

public:
	Operator(char *op, Operator * const lop, Operator * const rop);

public:
	void setLeft(Operator * const oper);
	void setRight(Operator * const oper);
	const bool setLoc(char * const loc);
	static const bool isValidNum(const char c);					//returns true if the character is a valid number character (numeral, '-', '.')
	static const op_t isValid(const string s, string &dest);	//returns a parsed string, if the arguments are valid. If invalid, it will return an empty string
	static const string isValid(const string s, op_t &dest);	//~
	//static const op_t isValid(const string s);
	static const string isValid(const string s);
	
	const Operator * const getLeft() const;
	const Operator * const getRight() const;
	char * const getLoc() const;
	const op_t getOpType() const;
	const double getResult() throw(int);	//returns the result of the Operator, will call getResult of Operators if they aren't NULL.
											//Throws 1 if the function cannot make the calculation.
											//Throws 2 if a function of one instance is called twice.
	double *getVariable();					//returns the address of the result value.
	const opSymb * const getFunc() const;
private:
	bool curCalc;	//used to regulate how many times a single instance of getResult is called
	op_t opType;	//used to regulate whether returning a varying pointer to the result value is allowed.
};



#endif /*OPERATOR_H_*/
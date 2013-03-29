#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "Operator.h"


class Expression
{
public:
	typedef vector<Operator *>::iterator OpIter;
	char *raw;									//raw user input
	string proc;								//processed user input into readable content
	vector<Operator *> conv;					//vector/array of pointers to Operators in proc
	unordered_map<char, vector<double *>> variables;	//database of char->double addresses representing the variables
private:
	void process();	//processes the raw string into a readable string
	void convert();	//converts the processed string into a series of Operators
	enum { RIGHT=0x0001, SKIP=0x0002 };		//returns the Operator which is the Operand of the Operator pointed to by init
	OpIter getOperand(OpIter init, unsigned short flags);
public:
	Expression(char * const init);
	void setRaw(char * const s);
}


#endif /*EXPRESSION_H_*/
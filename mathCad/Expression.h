#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "Operator.h"


class Expression
{
public:
	typedef vector<Operator *>::iterator OpIter;
	typedef unordered_map<char, vector<double *>>::iterator VarIter;
	typedef unordered_map<char, double> Arg_t;
private:
	char *raw;									//raw user input
	string proc;								//processed user input into readable content
	vector<Operator *> conv;					//vector/array of pointers to Operators in proc
	unordered_map<char, vector<double *>> variables;	//database of char->double addresses representing the variables
protected:
	static string * const process(char * const s);		//processes the raw string into a readable string
	static const bool validate(string &s);				//validates the processed string to see if it's convertable
	static vector<Operator *> * const convert(string &s, unordered_map<char, vector<double *>> &vars);//converts the processed string into a series of Operators
	enum { RIGHT=0x0001, SKIP=0x0002 };		//returns the Operator which is the Operand of the Operator pointed to by init
	static const OpIter getOperand(vector<Operator *> &expr, const OpIter &init, const unsigned short flags);
	static const bool link(vector<Operator *> &expr);
public:
	Expression(char * const init);
	Expression(string &init);

	const bool setRaw(char * const s);

	double evaluate();	//Default; Sets all variables to 0;
	double evaluate(Arg_t *args);

	void print(void);
};


#endif /*EXPRESSION_H_*/
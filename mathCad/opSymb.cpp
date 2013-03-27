#include "opSymb.h"

unordered_map<string, opSymb *> opSymb::valid;	//definition of previously declared unordered map


opSymb::opSymb(const ARG0 func, const unsigned short p): prior(p), args(0)
{
	opFunc.arg0 = func;
}

opSymb::opSymb(const ARG1 func, const unsigned short p): prior(p), args(1)
{
	opFunc.arg1 = func;
}

opSymb::opSymb(const ARG2 func, const unsigned short p): prior(p), args(2)
{
	opFunc.arg2 = func;
}

const unsigned short opSymb::getArguments()
{
	return args;
}

const unsigned short opSymb::getPriority()
{
	return prior;
}

OPFUNC opSymb::getFunc()
{
	return opFunc;
}

//Index of functions
double add(double a, double b)
{
	return a+b;
}

double mult(double a, double b)
{
	return a*b;
}

double div(double a, double b)
{
	return a/b;
}

double par(double a)
{
	return a;
}

void opSymb::loadValid()
{
	valid["+"] = new opSymb(add, ADD_PRIORITY);
	valid["*"] = new opSymb(mult, MULT_PRIORITY);
	valid["/"] = new opSymb(div, DIV_PRIORITY);
	valid["^"] = new opSymb(pow, POW_PRIORITY);
	valid["("] = new opSymb(par, PAR_PRIORITY);
}
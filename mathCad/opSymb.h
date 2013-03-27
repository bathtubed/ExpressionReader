#ifndef OP_SYMB_H_
#define OP_SYMB_H_

#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <math.h>
using namespace std;

#define PAR_PRIORITY	0
#define SYMB_PRIORITY	PAR_PRIORITY
#define POW_PRIORITY	SYMB_PRIORITY+10
#define MULT_PRIORITY	POW_PRIORITY+10
#define DIV_PRIORITY	MULT_PRIORITY
#define ADD_PRIORITY	MULT_PRIORITY+10


typedef double (* ARG0)();					//used for functions which get constants such as e, or PI
typedef double (* ARG1)(double);			//used for functions which return math functions which take 1 argument such as sin or cos or par
typedef double (* ARG2)(double, double);	//used for functions which return math functions which take 2 arguemnts suhc as + or * or ^
typedef union
{
	ARG0 arg0;
	ARG1 arg1;
	ARG2 arg2;
} OPFUNC;	//union of the three function types for storage purposes and aliuses.

class opSymb
{
public:
	static unordered_map<string, opSymb *> valid;		//hash map containing name-function values
	static void loadValid();	//loads valid with predefined values

private:
	OPFUNC opFunc;			//pointer to this operation symbol function
	unsigned short args;	//number of arguments this symbol has
	unsigned short prior;	//priority of the operation

public:
	opSymb(const ARG0 func, const unsigned short p);
	opSymb(const ARG1 func, const unsigned short p);
	opSymb(const ARG2 func, const unsigned short p);

public:	//get functions
	const OPFUNC getFunc();
	const unsigned short getPriority();
	const unsigned short getArguments();
};

/*
***************   Version 1 using std::vector. **************************************************************************
class opSymb	//manages viable operations readable by the program
{
public:
	static vector<opSymb *> valid;	//vector of all the valid functions loaded from `loadValid`
	static void loadValid();  //reads name:address of functions in filename to put in valid
private:
	string symb;	//symbol which appears in user input (+, sin, getPi)
	OPFUNC opFunc;	//
public:
	opSymb(const string s, const ARG0 func): symb(s) { opFunc.arg0 = func; };
	opSymb(const string s, const ARG1 func): symb(s) { opFunc.arg1 = func; };
	opSymb(const string s, const ARG2 func): symb(s) { opFunc.arg2 = func; };
public:
	const bool operator== (const string s);
};
*/


#endif  /*OP_SYMB_H_*/
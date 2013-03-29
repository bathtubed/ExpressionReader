#include "Operator.h"

Operator::Operator(char *op=NULL, Operator * const lop=NULL, Operator * const rop=NULL): lOperand(lop), rOperand(rop), result(0), curCalc(false)
{
	if(op==NULL)
		opLoc=NULL;
	else
	{
		if(!setLoc(op))
		{
			printf("Operator constructor was passed with invalid operation symbol\n");

			opLoc=NULL;
		}
	}
}

const bool Operator::isValidNum(const char c)
{
	if(	c == '.' ||
		(c <= '9' && c >= '0' ))
		return true;
	else
		return false;
}

const Operator::op_t Operator::isValid(const string s, string &dest)
{
	if(isValidNum(s[0]) || (s[0]=='-' && isValidNum(s[1])))
	{
		string::const_iterator beginIter = s.begin(), endIter = find_if_not(s.begin()+1, s.end(), isValidNum);
		dest.assign(beginIter, endIter);
		return NUM;
	}

	for(string::const_iterator i = s.begin(); i != s.end(); i++)
	{
		try
		{
			dest.assign(s.begin(), i);
			opSymb::valid.at(dest);
		}
		catch(out_of_range)
		{
			continue;
		}
		return SYM;
	}

	if((s[0] >= 'a' && s[0] <= 'z') ||
		(s[0] >= 'A' && s[0] <= 'Z'))
	{
		dest[0] = s[0];
		return VAR;
	}
	else
	{
		dest.clear();
		return INVALID;
	}
}


const string Operator::isValid(const string s, op_t &dest)
{
	string rtrn;
	dest = isValid(s, rtrn);
	return rtrn;
}

/*
const Operator::op_t Operator::isValid(const string s)
{
	string garbage;
	return isValid(s, garbage);
}
*/

const string Operator::isValid(const string s)
{
	op_t garbage;
	return isValid(s, garbage);
}

inline void Operator::setLeft(Operator * const oper)
{
	lOperand = oper;
}

inline void Operator::setRight(Operator * const oper)
{
	rOperand = oper;
}

const bool Operator::setLoc(char * const loc)
{
	string symbol;
	switch(opType = isValid(loc, symbol))
	{
	case INVALID:
		opLoc = NULL;
		return false;
	case NUM:
		result		= atof(symbol.c_str());
		opFunc		= NULL;
		opLoc		= loc;
		break;
	case VAR:
		result		= 0;
		opFunc		= NULL;
		opLoc		= loc;
		break;
	case SYM:
		result		= 0;
		opFunc		= opSymb::valid[symbol];
		opLoc		= loc;
		break;
	default:
		break;
	}
	
	return true;
}

inline const Operator * const Operator::getLeft() const
{
	return lOperand;
}

inline const Operator * const Operator::getRight() const
{
	return rOperand;
}

inline char * const Operator::getLoc() const
{
	return opLoc;
}

inline const Operator::op_t Operator::getOpType() const
{
	return opType;
}

double *Operator::getVariable()
{
	if(opType == VAR)
		return &result;
	else
		return NULL;
}

const opSymb * const Operator::getFunc() const
{
	return opFunc;
}

const double Operator::getResult()
{
	if(curCalc)
	{
		/****** Uncomment in debug mode only ****************************************

		printf("Operator::getResult called itself through recursion at some point.\n \
Tests are being done from function of object who had said function called a second time.\n\n");
		if(this == lOperand)
			printf("The left operand of this operator points to itself.\n");
		if(this == rOperand)
			printf("The right operand of this operator points to itself.\n");
		if(lOperand == rOperand)
			printf("The left and right operands point to the same operators.\n");
		*/
		throw 2;
	}
	else
		curCalc = true;
	if(opType == INVALID)
		throw 1;
	if(opType == SYM)
	{
		switch(opFunc->getArguments())
		{
		case 0:
			result = opFunc->func();
			break;
		case 1:
			result = opFunc->func(rOperand->getResult());
			break;
		case 2:
			result = opFunc->func(lOperand->getResult(), rOperand->getResult());
			break;
		}
	}
	curCalc = false;
	return result;
}
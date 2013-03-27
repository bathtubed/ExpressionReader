#include "Operator.h"

Operator::Operator(char *op=NULL, Operator * const lop=NULL, Operator * const rop=NULL): lOperand(lop), rOperand(rop)
{
	if(op==NULL)
		opLoc=NULL;
	else
	{
		if(setLoc(op))
		{
			printf("Operator constructor was passed with invalid operation symbol\n");
			opLoc=NULL;
		}
	}
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
	if(isValidNum(*loc))
	{
		int i;
		string num;
		for(i=0; isValidNum(loc[i]); i++);
		num.assign(loc, i);
	}

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
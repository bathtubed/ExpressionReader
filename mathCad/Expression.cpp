#include "Expression.h"

Expression::Expression(char * const init)
{
	setRaw(init);
}

void Expression::setRaw(char * const s)
{
	raw = s;
	process();
	convert();
	for(vector<Operator *>::iterator i = conv.begin(); i != conv.end(); i++)
	{
		(*i)->setLeft(*getOperand(i, 0));
		(*i)->setRight(*getOperand(i, RIGHT));
	}
}

void Expression::process()
{
	if(raw == NULL)
	{
		printf("Bad arguments passed");
		return;
	}
	
	proc = raw;
	string garbage;
	for(string::iterator i = proc.begin(); i != proc.end(); i++)
	{
		if(*i != '+' && *(i+1) == '-' && (Operator::isValid(&(*(i+2)), garbage) != Operator::INVALID))
			proc.replace(i+1, i+2, "+-");
	}

	for(string::iterator i = proc.begin(); i != proc.end(); i++)
	{
		if(*i == '-' && !Operator::isValidNum(*(i+1)))
			proc.replace(i, i+1, "-1*");
	}

	proc.insert(proc.begin(), '(');
	proc.append(")");

}

void Expression::convert()
{
	Operator::op_t T;
	for(string::iterator i = proc.begin(); i != proc.end(); i++)
	{
		if(!Operator::isValid(&(*i), T).empty())
		{
			conv.push_back(new Operator(&(*i), NULL, NULL));
			if(T == Operator::VAR && conv.back()->getVariable() != NULL)
				variables[*conv.back()->getLoc()].push_back(conv.back()->getVariable());
		}
	}
}
#include "Expression.h"

Expression::Expression(char * const init)
{
	setRaw(init);
}

void Expression::setRaw(char * const s)
{
	if(raw == NULL)
	{
		printf("(from Expression::setRaw())\n > Bad args\n");
		return;
	}
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
		printf("(from Expression::process())\n > NULL data");
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
	if(proc.empty())
	{
		printf("(from Expression::convert())\n > Empty data");
		return;
	}
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

Expression::OpIter Expression::getOperand(Expression::OpIter init, unsigned short flags)
{
	if(conv.empty())
	{
		printf("(from Expression::getOperand())\n > unprepared");
		return;
	}
	bool right = flags & RIGHT;
	bool skip = flags & SKIP;
	bool high = *(*init)->getLoc() == (right? '(':')');
	OpIter i, best = conv.end();
	for(i = init+(right? 1:-1); *(*i)->getLoc() != (right? ')':'('); right? ++i:--i)
	{
		if(!skip)
		{
			if((*i)->getFunc()->getPriority() <= (high? 0:((*init)->getFunc()->getPriority())) &&
				(*i)->getFunc()->getPriority() > (best == conv.end()? 0:(*best)->getFunc()->getPriority()))
				best = i;
			if((*i)->getFunc()->getPriority() >= (high? 0:((*init)->getFunc()->getPriority())))
				break;
		}
		if(*(*i)->getLoc() == (right? '(':')'))
			i = getOperand(i, (flags & RIGHT) | SKIP);
	}
}
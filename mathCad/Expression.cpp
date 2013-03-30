#include "Expression.h"

Expression::Expression(char * const init)
{
	setRaw(init);
}

Expression::Expression(string init)
{
	setRaw(const_cast<char *>(init.c_str()));
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
		switch((*i)->getFunc()->getArguments())
		{
		case 2:
			(*i)->setLeft(*getOperand(i, 0));
		case 1:
			(*i)->setRight(*getOperand(i, RIGHT));
		case 0:
			break;
		}
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
		if(*i == ' ')
			proc.erase(i);
	}

	for(string::iterator i = proc.begin(); (i+2) != proc.end(); i++)
	{
		if(Operator::isValid(&*i, garbage) != Operator::SYM && *(i+1) == '-' && (Operator::isValid(&(*(i+2)), garbage) != Operator::INVALID))
			proc.replace(i+1, i+2, "+-");
	}

	for(string::iterator i = proc.begin(); i+1 != proc.end(); i++)
	{
		if(*i == '-' && !Operator::isValidNum(*(i+1)))
			proc.replace(i, i+1, "-1*");
		if((Operator::isValid(&*(i+1), garbage) != Operator::INVALID && (Operator::isValid(&*(i+1), garbage) != Operator::SYM || *i=='(')) &&
			(Operator::isValid(&*i, garbage) == Operator::VAR || Operator::isValidNum(*i) || *i==')') &&
			!(Operator::isValid(&*i, garbage) == Operator::NUM && Operator::isValid(&*(i+1), garbage) == Operator::NUM))
			proc.insert(i+1, '*');
		
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
	string symb;
	for(string::iterator i = proc.begin(); i != proc.end(); i++)
	{
		if(!(symb=Operator::isValid(&(*i), T)).empty())
		{
			conv.push_back(new Operator(&(*i), NULL, NULL));
			if(T == Operator::VAR && conv.back()->getVariable() != NULL)
				variables[*conv.back()->getLoc()].push_back(conv.back()->getVariable());
			for(string::iterator j = symb.begin(); j != symb.end(); j++, i++);
			i--;
		}
	}
}

Expression::OpIter Expression::getOperand(Expression::OpIter init, unsigned short flags)
{
	if(conv.empty())
	{
		printf("(from Expression::getOperand())\n > unprepared");
		return conv.end();
	}
	bool right = flags & RIGHT;
	bool skip = flags & SKIP;
	bool high = *(*init)->getLoc() == (right? '(':')');
	OpIter i, best = conv.end();
	for(i = init+(right? 1:-1); i != (right? conv.end():conv.begin()) && (!right? (*(*i)->getLoc() != '('):(*((*i)->getLoc()-1) != ')')); right? ++i:--i)
	{
		if(!skip)
		{
			if(right? ((*i)->getFunc()->getPriority() >= (high? HIGH_PRIORITY:((*init)->getFunc()->getPriority()))):
				((*i)->getFunc()->getPriority() > (high? HIGH_PRIORITY:((*init)->getFunc()->getPriority()))))
				break;
			if((*i)->getFunc()->getPriority() <= (high? HIGH_PRIORITY:((*init)->getFunc()->getPriority())) &&
				(right? ((*i)->getFunc()->getPriority() >= (best == conv.end()? -1:(*best)->getFunc()->getPriority())):
				((*i)->getFunc()->getPriority() > (best == conv.end()? -1:(*best)->getFunc()->getPriority()))))
				best = i;
		}

		if(right? (*(*i)->getLoc() == '('):(*((*i)->getLoc()-1) == ')'))
		{
			i = getOperand(i, (flags & RIGHT) | SKIP);
			right? --i:++i;
		}
	}

	return skip? i:best;
}

double Expression::evaluate(Arg_t *args)
{
	for(VarIter i = variables.begin(); i != variables.end(); i++)
	{
		try
		{
			for(vector<double *>::iterator j = i->second.begin(); j != i->second.end(); **(j++)=args->at(i->first));
		}
		catch(out_of_range)
		{
			printf("Expression::evaluate(unordered_map<char, double>args) was passed with an insufficient argument list");
			return 0.0;
		}
	}
	try
	{
		return conv[0]->getResult();
	}
	catch(int e)
	{
		if(e==2)
			printf("\nExpression class failed\n");
		return 0.0;
	}
}

double Expression::evaluate()
{
	Arg_t *args = new Arg_t;
	for(VarIter i = variables.begin(); i != variables.end(); i++)
		(*args)[i->first] = 0.0;
	return evaluate(args);
}
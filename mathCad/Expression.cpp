#include "Expression.h"

Expression::Expression(char * const init)
{
	setRaw(init);
}

Expression::Expression(string &init)
{
	if(!setRaw(const_cast<char *>(init.c_str())))
	{
		printf("Invalid initialization\n");
		throw 42;
	}
}

const bool Expression::setRaw(char * const s)
{
	if(s == NULL)
	{
		printf("(from Expression::setRaw())\n > Bad args\n");
		return false;
	}
	raw = s;
	proc = *process(raw);
	if(validate(proc))
	{
		conv = *convert(proc, variables);
		link(conv);
	}
	else
		return false;
	
	return true;
}

const bool Expression::link(vector<Operator *> &expr)
{
	for(vector<Operator *>::iterator i = expr.begin(); i != expr.end(); i++)
	{
		switch((*i)->getFunc()->getArguments())
		{
		case 2:
			(*i)->setLeft(*getOperand(expr, i, 0));
		case 1:
			(*i)->setRight(*getOperand(expr, i, RIGHT));
		case 0:
			break;
		}
	}
	return true;
}

string * const Expression::process(char * const s)
{
	if(s == NULL)
	{
		printf("(from Expression::process())\n > NULL data");
		return NULL;
	}
	
	string *rtrn = new string(s);
	string garbage;

	rtrn->erase(remove(rtrn->begin(), rtrn->end(), ' '), rtrn->end());

	for(string::iterator j = rtrn->begin(); (j+2) != rtrn->end(); j++)
	{
		if(Operator::isValid(&*j, garbage) != Operator::SYM && *(j+1) == '-' && (Operator::isValid(&(*(j+2)), garbage) != Operator::INVALID))
			rtrn->replace(j+1, j+2, "+-");
	}

	rtrn->insert(rtrn->begin(), '(');
	rtrn->append(")");
	for(string::iterator k = rtrn->begin(); k+1 != rtrn->end(); k++)
	{
		if(*k == '-' && !Operator::isValidNum(*(k+1)))
			rtrn->replace(k, k+1, "-1*");
		if((Operator::isValid(&*(k+1), garbage) != Operator::INVALID && (Operator::isValid(&*(k+1), garbage) != Operator::SYM || *(k+1)=='(')) &&
			(Operator::isValid(&*k, garbage) == Operator::VAR || Operator::isValidNum(*k) || *k==')') &&
			!(Operator::isValid(&*k, garbage) == Operator::NUM && Operator::isValid(&*(k+1), garbage) == Operator::NUM))
		{
			if(Operator::isValid(&*k, garbage) == Operator::SYM)
			{
				if(opSymb::valid[garbage]->getArguments())
					k = rtrn->insert(k+1, '*')-1;
			}
			else
			{
				k = rtrn->insert(k+1, '*')-1;
			}
		}
	}

	return rtrn;
}

const bool Expression::validate(string &s)
{
	int paren=0;
	for(string::iterator i = s.begin(); i != s.end(); i++)
	{
		if(*i == '(')
			paren++;
		else if(*i == ')')
			paren--;
	}
	
	return !paren;
}

vector<Operator *> * const Expression::convert(string &s, unordered_map<char, vector<double *>> &vars)
{
	if(s.empty())
	{
		printf("(from Expression::convert())\n > Empty data");
		return NULL;
	}

	vector<Operator *> *rtrn = new vector<Operator *>;
	Operator::op_t T;
	string symb;
	for(string::iterator i = s.begin(); i != s.end(); i++)
	{
		if(!(symb=Operator::isValid(&(*i), T)).empty())
		{
			rtrn->push_back(new Operator(&(*i), NULL, NULL));
			if(T == Operator::VAR && rtrn->back()->getVariable() != NULL)
				vars[*rtrn->back()->getLoc()].push_back(rtrn->back()->getVariable());
			for(string::iterator j = symb.begin(); j != symb.end(); j++, i++);
			i--;
		}
	}

	return rtrn;
}

const Expression::OpIter Expression::getOperand(vector<Operator *> &expr, const Expression::OpIter &init, unsigned short flags)
{
	if(expr.empty())
	{
		printf("(from Expression::getOperand())\n > unprepared");
		return expr.end();
	}
	bool right = flags & RIGHT;
	bool skip = flags & SKIP;
	bool high = *(*init)->getLoc() == (right? '(':')');
	bool rep = true;
	OpIter i, best = expr.end();
	if(*((*init)->getLoc()-1) == ')' && !right && !skip)
	{
		i = getOperand(expr, init, SKIP);
		best = i;
		i--;
	}
	else
		i = init+(right? 1:-1);
	for( ; i != (right? expr.end():expr.begin()) && (rep? (!right? (*(*i)->getLoc() != '('):(*((*i)->getLoc()-1) != ')')):true); right? ++i:--i)
	{
		rep = true;
		if(!skip)
		{
			if(right? ((*i)->getFunc()->getPriority() >= (high? HIGH_PRIORITY:((*init)->getFunc()->getPriority()))):
				((*i)->getFunc()->getPriority() > (high? HIGH_PRIORITY:((*init)->getFunc()->getPriority()))))
				break;
			if((*i)->getFunc()->getPriority() <= (high? HIGH_PRIORITY:((*init)->getFunc()->getPriority())) &&
				(right? ((*i)->getFunc()->getPriority() >= (best == expr.end()? -1:(*best)->getFunc()->getPriority())):
				((*i)->getFunc()->getPriority() > (best == expr.end()? -1:(*best)->getFunc()->getPriority()))))
				best = i;
		}

		if(right? (*(*i)->getLoc() == '('):(*((*i)->getLoc()-1) == ')'))
		{
			i = getOperand(expr, i, (flags & RIGHT) | SKIP);
			right? i--:i++;
			rep = false;
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

void Expression::print(void)
{
	printf("raw input: %s\n", raw);
	printf("processed input: %s\n", proc.c_str());
}
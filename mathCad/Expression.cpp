#include "Expression.h"

Expression::Expression(char * const init)
{
	setRaw(init);
}

Expression::Expression(string &init)
{
	setRaw(const_cast<char *>(init.c_str()));
}

void Expression::setRaw(char * const s)
{
	if(s == NULL)
	{
		printf("(from Expression::setRaw())\n > Bad args\n");
		return;
	}
	raw = s;
	proc = *process(raw);
	conv = *convert(proc, variables);
	link(conv);
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
	for(string::iterator i = rtrn->begin(); i != rtrn->end(); i++)
	{
		if(*i == ' ')
			rtrn->erase(i);
	}

	for(string::iterator i = rtrn->begin(); (i+2) != rtrn->end(); i++)
	{
		if(Operator::isValid(&*i, garbage) != Operator::SYM && *(i+1) == '-' && (Operator::isValid(&(*(i+2)), garbage) != Operator::INVALID))
			rtrn->replace(i+1, i+2, "+-");
	}

	rtrn->insert(rtrn->begin(), '(');
	rtrn->append(")");
	for(string::iterator i = rtrn->begin(); i+1 != rtrn->end(); i++)
	{
		if(*i == '-' && !Operator::isValidNum(*(i+1)))
			rtrn->replace(i, i+1, "-1*");
		if((Operator::isValid(&*(i+1), garbage) != Operator::INVALID && (Operator::isValid(&*(i+1), garbage) != Operator::SYM || *i=='(')) &&
			(Operator::isValid(&*i, garbage) == Operator::VAR || Operator::isValidNum(*i) || *i==')') &&
			!(Operator::isValid(&*i, garbage) == Operator::NUM && Operator::isValid(&*(i+1), garbage) == Operator::NUM))
		{
			if(Operator::isValid(&*i, garbage) == Operator::SYM)
			{
				if(opSymb::valid[garbage]->getArguments())
					rtrn->insert(i+1, '*');
			}
			else
				rtrn->insert(i+1, '*');
		}
		
	}

	return rtrn;
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
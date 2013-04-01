#include <cstdlib>
#include <iostream>
#include "Expression.h"

int main(void)
{
	opSymb::loadValid();
	string expression("(2x+0)");
	Expression *expr;
	try
	{
		expr = new Expression(expression);
	}
	catch(int e)
	{
		if(e == 42)
		{
			printf("Bad Expression\n");
			system("pause");
			return 0;
		}
	}
	Expression::Arg_t *args = new Expression::Arg_t;
	(*args)['x'] = 3;
	expr->print();
	cout<<expr->evaluate(args)<<endl;
	system("pause");
	return 0;
}
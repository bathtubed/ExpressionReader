#include <cstdlib>
#include <iostream>
#include "Expression.h"

int main(void)
{
	opSymb::loadValid();
	string expression("3x^2 + 5y - 9");
	Expression *expr = new Expression(expression);
	Expression::Arg_t *args = new Expression::Arg_t;
	(*args)['x'] = 3.0;
	(*args)['y'] = -6.5;
	cout<<expression<<endl<<expr->evaluate(args)<<endl;
	system("pause");
	return 0;
}
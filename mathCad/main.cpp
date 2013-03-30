#include <cstdlib>
#include <iostream>
#include "Expression.h"

int main(void)
{
	opSymb::loadValid();
	string expression("3x^2 + 5x - 9");
	Expression *expr = new Expression(expression);
	Expression::Arg_t *args = new Expression::Arg_t;
	(*args)['x'] = 3.0;
	cout<<expression<<endl<<expr->evaluate(args)<<endl;
	system("pause");
	return 0;
}
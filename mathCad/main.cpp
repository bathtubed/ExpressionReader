#include <cstdlib>
#include <iostream>
#include "Expression.h"

int main(void)
{
	opSymb::loadValid();
	string expression("10x^3 + 5x^2 - (5/3)x + PI");
	Expression *expr = new Expression(expression);
	Expression::Arg_t *args = new Expression::Arg_t;
	(*args)['x'] = 3;
	cout<<expression<<endl<<expr->evaluate(args)<<endl;
	system("pause");
	return 0;
}
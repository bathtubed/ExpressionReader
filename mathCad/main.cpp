#include <cstdlib>
#include <iostream>
#include "Expression.h"

int main(void)
{
	opSymb::loadValid();
	string expression("(8x^(4-2)+20)(12+x/8)");
	Expression *expr = new Expression(expression);
	Expression::Arg_t *args = new Expression::Arg_t;
	(*args)['x'] = 3;
	cout<<expression<<endl<<expr->evaluate(args)<<endl;
	system("pause");
	return 0;
}
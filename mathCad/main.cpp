#include <cstdlib>
#include <iostream>
#include "Expression.h"

int main(void)
{
	opSymb::loadValid();
	string expression("4.93^2 * -(6+3/2)");
	Expression *expr = new Expression(expression);
	cout<<expression<<endl<<expr->evaluate()<<endl;
	system("pause");
	return 0;
}
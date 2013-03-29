#include "Operator.h"
#include <cstdlib>
#include <iostream>

int main(void)
{
	opSymb::loadValid();
	string expression("6/2*(2+PI)");
	Operator *div	= new Operator(&expression[1], new Operator(&expression[0], NULL, NULL), new Operator(&expression[2], NULL, NULL));
	Operator *plus	= new Operator(&expression[6], new Operator(&expression[5], NULL, NULL), new Operator(&expression[7], NULL, NULL));
	Operator *par	= new Operator(&expression[4], NULL, plus);
	Operator *total	= new Operator(&expression[3], div, par);
	try
	{
		cout<<expression<<" evaluates to >"<<endl<<total->getResult()<<endl;
	}
	catch(int e)
	{
		if(e==1)
			cout<<endl<<"Not prepared to make calculation"<<endl;
		else if(e==2)
			cout<<endl<<"As previously mentioned, getResult called itself"<<endl;
	}
	system("pause");
	return 0;
}
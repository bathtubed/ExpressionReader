#include "Operator.h"
#include <cstdlib>
#include <iostream>

int main(void)
{
	opSymb::loadValid();
	string expression("");
	Operator *plus = new Operator(&expression[1], new Operator(&expression[0], NULL, NULL), new Operator(&expression[2], NULL, NULL));
	try
	{
		cout<<expression<<" evaluates to >"<<endl<<plus->getResult()<<endl;
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
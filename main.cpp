#include "Grammar.h"

int main()
{
	Grammar g;
	g.init();
	
	g.first_letter_substitution();
	g.left_common_factor();
	g.printProduction();
	
	/*
	g.cal_First();
	g.printFirst();

	g.cal_Follow();
	g.printFollow();

	g.cal_Select();
	g.printSelect();

	if (g.is_LL1())
		g.printTable();
	else
		cout << "�����ķ�����LL(1)���ķ�" << endl;
	*/
	return 0;
}
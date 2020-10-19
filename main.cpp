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
		cout << "输入文法不是LL(1)型文法" << endl;
	*/
	return 0;
}
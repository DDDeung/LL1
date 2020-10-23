#include "Grammar.h"

int main()
{
	Grammar g;
	g.init();
	/*
	// ×î×ó·ÇÖÕ½á·û´úÈë
	g.first_letter_substitution();
	cout << "´úÈëºó£º" << endl;
	g.printProduction();

	// Ïû³ı×óµİ¹é
	g.parsing_left_recursion();
	cout << "Ïû³ı×óµİ¹é£º" << endl;
	g.printProduction();

	// ÌáÈ¡×ó¹«Òò×Ó
	g.left_common_factor();
	cout << "ÌáÈ¡×ó¹«Òò×Ó£º" << endl;
	g.printProduction();

	// É¾³ı²»¿É´ïµÄ²úÉúÊ½
	g.del_unreachable_production();
	g.printProduction();
	*/

	// ¼ÆËãFirst¼¯ºÏ
	g.cal_First();
	g.printFirst();

	// ¼ÆËãFollow¼¯ºÏ
	g.cal_Follow();
	g.printFollow();

	// ¼ÆËãSelect¼¯ºÏ
	g.cal_Select();
	g.printSelect();

	if (g.is_LL1())
	{
		g.printTable();
		if (g.grammar_parsing())
		{
			cout << "¾ä×ÓÆ¥Åä³É¹¦" << endl;
		}
	}
	else
<<<<<<< HEAD
		cout << "ÊäÈëÎÄ·¨²»ÊÇLL(1)ĞÍÎÄ·¨" << endl;

=======
		cout << "è¾“å…¥æ–‡æ³•ä¸æ˜¯LL(1)å‹æ–‡æ³•" << endl;
	*/
>>>>>>> 563fd49d0e9ab69e515faae5f407a511da1c58f6
	return 0;
}

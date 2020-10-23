#include "Grammar.h"

int main()
{
	Grammar g;
	g.init();
	/*
	// ������ս������
	g.first_letter_substitution();
	cout << "�����" << endl;
	g.printProduction();

	// ������ݹ�
	g.parsing_left_recursion();
	cout << "������ݹ飺" << endl;
	g.printProduction();

	// ��ȡ������
	g.left_common_factor();
	cout << "��ȡ�����ӣ�" << endl;
	g.printProduction();

	// ɾ�����ɴ�Ĳ���ʽ
	g.del_unreachable_production();
	g.printProduction();
	*/

	// ����First����
	g.cal_First();
	g.printFirst();

	// ����Follow����
	g.cal_Follow();
	g.printFollow();

	// ����Select����
	g.cal_Select();
	g.printSelect();

	if (g.is_LL1())
	{
		g.printTable();
		if (g.grammar_parsing())
		{
			cout << "����ƥ��ɹ�" << endl;
		}
	}
	else
<<<<<<< HEAD
		cout << "�����ķ�����LL(1)���ķ�" << endl;

=======
		cout << "输入文法不是LL(1)型文法" << endl;
	*/
>>>>>>> 563fd49d0e9ab69e515faae5f407a511da1c58f6
	return 0;
}

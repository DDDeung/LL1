#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
using namespace std;

// 产生式结构体
struct pstring {
	char left;
	string right;
};

class Grammar
{
public:
	Grammar();
	// 拷贝构造函数
	Grammar(const Grammar&);
	// 用户输入初始化文法
	void init();
	// 初始化产生式集合
	void init_P();			
	// 将产生式右侧第一个字符为非终结符的进行代入替换
	void first_letter_substitution();
<<<<<<< HEAD
	// �����ɴ�Ĳ���ʽɾ��
	void del_unreachable_production();
	// ��ȡ����ʽ
	void left_common_factor();
	// ������ݹ�
	void parsing_left_recursion();
	// ����First����
=======
	// 提取左公因式
	void left_common_factor();
	// 计算First集合
>>>>>>> 563fd49d0e9ab69e515faae5f407a511da1c58f6
	void cal_First();			
	// 计算Follow集合
	void cal_Follow();		
	// 调用递归方法计算Follow集合
	void cal_Follow_recur();
	// 计算Select集合
	void cal_Select();
	// 生成预测分析表，当同一单元格有多个式子时，返回false
	bool get_Table();
	// 判断是否为LL(1)文法
	bool is_LL1();
<<<<<<< HEAD
	// Ԥ������������﷨����
	bool grammar_parsing();
	// ��ĳ�����ս����First����
=======
	// 对某个非终结符求First集合
>>>>>>> 563fd49d0e9ab69e515faae5f407a511da1c58f6
	void get_First_recur(char);				
	// 不递归的求出Follow集合
	void get_Follow();					
	// 对某个非终结符求Follow集合
	void get_Follow_recur(char,int);	
	// 判断是否为终结符
	bool inVt(char);					
	// 判断是否为非终结符
	bool inVn(char);				
	// 求终结符在预测分析表中的列标
	int index_in_terminal(char);
	// 求非终结符在预测分析表中的行标
	int index_in_nonterminal(char);
	// 输出产生式集
	void printProduction();
	// 输出First集
	void printFirst();					
	// 输出Follow集
	void printFollow();					
	// 输出Select集
	void printSelect();			
	// 输出预测分析表
	void printTable();
	~Grammar();

private:
	// 终结符集，小写字母
	set<char> Vt;						
	// 非终结符集，大写字母
	set<char> Vn;						
	// 产生式集合P
	vector<pstring> P;					
	// 产生式中包含的终结符集
	set<char> terminal;					
	// 产生式中包含的非终结符集
	set<char> nonterminal;				
	// First集合
	map<char, set<char>> first_set;		
	// Follow集合
	map<char, set<char>> follow_set;	
	// Select集合
	vector<set<char>> select_set;		
	// 预测分析表
	int** predict_table;
	// 产生式数量
	int pnum;
<<<<<<< HEAD
	// ��ʼ����
	char S;		
	// LL(1)�ķ���־
	bool LL1;
};
=======
	// 开始符号
	char S;								
};
>>>>>>> 563fd49d0e9ab69e515faae5f407a511da1c58f6

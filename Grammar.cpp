#include "Grammar.h"

Grammar::Grammar()
{
	int i, j;

	#pragma region 初始化终结集与非终结集
	for (i = 97, j = 65; (i <= 122) && (j <= 90); i++, j++)
	{
		this->Vt.insert(char(i));
		this->Vn.insert(char(j));
	}
	// 特别的，终结符中用$表示空串，#表示结束符
	Vt.insert('$');
	Vt.insert('#');
	#pragma endregion

	this->pnum = 0;
	this->S = 'S';
	this->predict_table = new int* [1];
}

// 拷贝构造函数
Grammar::Grammar(const Grammar& g)
{
	this->Vn = g.Vn;
	this->Vt = g.Vt;
	this->P = g.P;
	this->terminal = g.terminal;
	this->nonterminal = g.nonterminal;
	this->first_set = g.first_set;
	this->follow_set = g.follow_set;
	this->select_set = g.select_set;
	this->pnum = g.pnum;
	this->S = g.S;
}

// 用户输入初始化文法
void Grammar::init()
{
	#pragma region 初始化开始符号和产生式集合
	// 用户输入开始符号
	char start;
	while (true)
	{
		cout << "请输入开始符号：";
		cin >> start;
		if (inVn(start))
		{
			this->S = start;
			break;
		}
		else
		{
			cout << "开始符号应在非终结集中！请重新输入" << endl;
		}
	}
	cout << endl;

	// 用户输入产生式集合
	init_P();
	#pragma endregion
}

// 初始化产生式集合
void Grammar::init_P()
{
	int i, flag;
	this->pnum = 0;
	cout << "请输入产生式：(输入0结束)" << endl;
	cout << "形如:S-aA，空串以$表示" << endl << endl;
	while (true)
	{
		flag = 0;
		string p;
		cin >> p;
		if (p == "0")
		{
			break;
		}
		else
		{
			char left = p[0];
			this->nonterminal.insert(left);
			if (!inVn(left))
			{
				cout << "产生式左端应为非终结符！请重新输入" << endl << endl;
			}
			else
			{
				int i = p.find("-");
				string right;
				right = p.substr(i + 1, p.length());
				for (i = 0; i < right.length(); i++)
				{
					if (!inVt(right[i]) && !inVn(right[i]))
					{
						cout << "产生式左部含非法字符！";
						flag = 1;
						break;
					}
					if (inVt(right[i]))
					{
						this->terminal.insert(right[i]);
					}
				}

				for (i = 0; i < this->P.size(); i++)
				{
					if (this->P[i].left == left && this->P[i].right == right)
					{
						// 重复标志
						flag = 1;
						break;
					}
				}

				if (flag != 1)
				{
					pstring temp;
					temp.left = left;
					temp.right = right;
					this->P.push_back(temp);
					this->pnum++;
					cout << "产生式添加成功" << endl << endl;
				}
				else
				{
					cout << "请输入新的产生式：" << endl << endl;
				}
			}
		}
	}
}

// 将产生式右侧第一个字符为非终结符的进行代入替换
void Grammar::first_letter_substitution()
{
	int i, j;
	int flag = 1;
	while (flag)
	{
		flag = 0;
		for (i = 0; i < this->pnum; i++)
		{
			char r0 = this->P[i].right[0];

			if (inVn(r0))
			{
				flag = 1;
				string right;
				if (this->P[i].right.length() > 1)
				{
					right = this->P[i].right.substr(1, this->P[i].right.length());
				}
				else
				{
					right = "";
				}

				for (j = 0; j < this->pnum; j++)
				{
					// i==j是左递归的情况，代入这里不做处理
					if (this->P[j].left == r0 && i != j)
					{
						pstring newp;
						newp.left = this->P[i].left;
						if (this->P[j].right == "$")
						{
							newp.right += right;
						}
						else
						{
							newp.right = this->P[j].right + right;
						}
						if (newp.right.length() == 0)
							newp.right += "$";
						this->P.push_back(newp);
					}
				}
			}

			if (flag == 1)
			{
				this->P.erase(this->P.begin() + i);
				this->pnum = this->P.size();
				break;
			}
		}
	}
}

// 提取左公因式
void Grammar::left_common_factor()
{
	int i, j;
	char new_nt = 90;	// 新增非终结符，从Z开始向A加入
	int flag = 1;		// 如果有左公因式，那就要对产生式集合做出改变，flag置1
	while (flag != 0)
	{
		flag = 0;
		set<int> lcf;	// 用一个set记录拥有左公因子的产生式下标，倒序删除就不会打乱
		for (i = 0; i < this->pnum; i++)
		{
			lcf.clear();
			char left = this->P[i].left;
			string right;
			int length;
			for (length = this->P[i].right.length(); length >= 1; length--)
			{
				lcf.clear();
				lcf.insert(i);
				right = this->P[i].right.substr(0, length);
				for (j = 0; j < this->pnum; j++)
				{
					if (j != i)
					{
						if (this->P[j].left == left)
						{
							if (this->P[j].right.length() >= length)
							{
								if (this->P[j].right.substr(0, length) == right)
								{
									lcf.insert(j);
								}
							}
						}
					}
				}
				if (lcf.size() > 1)
				{
					flag = 1;
					while (this->nonterminal.find(new_nt) != this->nonterminal.end())
						new_nt--;
					this->nonterminal.insert(new_nt);

					set<int>::reverse_iterator lcf_iter;
					for (lcf_iter = lcf.rbegin(); lcf_iter != lcf.rend(); lcf_iter++)
					{
						string new_right;
						if (this->P[*lcf_iter].right.length() == length)
						{
							new_right = '$';	// S->ab，提取ab后，Z->ε
						}
						else
						{
							new_right = this->P[*lcf_iter].right.substr(length, this->P[*lcf_iter].right.length());  // S->abB，提取ab后，Z->B
						}
						pstring new_p;
						new_p.left = new_nt;
						new_p.right = new_right;
						this->P.erase(this->P.begin() + *lcf_iter);
						this->P.push_back(new_p);
					}

					pstring new_p2;
					new_p2.left = left;
					new_p2.right += right;
					new_p2.right += new_nt--;
					this->P.push_back(new_p2);
				}

				if (flag == 1)
				{
					this->pnum = this->P.size();
					break;
				}
			}

			if (flag == 1)
			{
				break;
			}
		}
	}
}

// 计算First集合
void Grammar::cal_First()
{
	// 首先给每个非终结符初始化一个空的First集合
	set<char>::iterator nterm_iter;
	for (nterm_iter = this->nonterminal.begin(); nterm_iter != this->nonterminal.end(); nterm_iter++)
	{
		set<char> value;
		this->first_set.insert(pair<char, set<char>>(*nterm_iter, value));
	}
	// 对每个非终结符求First集合
	for (nterm_iter = this->nonterminal.begin(); nterm_iter != this->nonterminal.end(); nterm_iter++)
	{
		get_First_recur(*nterm_iter);
	}
}

// 调用递归方法计算Follow集合
void Grammar::cal_Follow_recur()
{
	// 首先给每个非终结符初始化Follow集合，特别的对S添加#结束符
	set<char>::iterator nterm_iter;
	for (nterm_iter = this->nonterminal.begin(); nterm_iter != this->nonterminal.end(); nterm_iter++)
	{
		set<char> value;
		if (*nterm_iter == this->S)
		{
			value.insert('#');
		}
		this->follow_set.insert(pair<char, set<char>>(*nterm_iter, value));
	}
	
	// 对每个非终结符求Follow集合
	for (nterm_iter = this->nonterminal.begin(); nterm_iter != this->nonterminal.end(); nterm_iter++)
	{
		get_Follow_recur(*nterm_iter, 1);
	}
}

// 不使用递归计算Follow集合
void Grammar::cal_Follow()
{
	// 首先给每个非终结符初始化Follow集合，特别的对S添加#结束符
	set<char>::iterator nterm_iter;
	for (nterm_iter = this->nonterminal.begin(); nterm_iter != this->nonterminal.end(); nterm_iter++)
	{
		set<char> value;
		if (*nterm_iter == this->S)
		{
			value.insert('#');
		}
		this->follow_set.insert(pair<char, set<char>>(*nterm_iter, value));
	}

	get_Follow();
}

// 计算Select集合
void Grammar::cal_Select()
{
	int i, j;
	for (i = 0; i < this->pnum; i++)
	{
		int countEmpty = 0;			// 右端全部能推出$，则使用 Select(A-a) = First(a)-$ ∪ Follow(A)
		set<char> select;
		for (j = 0; j < this->P[i].right.length(); j++)
		{
			char rj = this->P[i].right[j];
			// 遇到终结符
			if (inVt(rj))		
			{
				if (rj != '$')	// 不是空串，则直接加入并结束
					select.insert(this->P[i].right[j]);
				else if (rj == '$')
					countEmpty++;
				break;
			}
			// 遇到非终结符
			else
			{
				// 将其First集合中非$符加入
				set<char>::iterator value_iter;
				for (value_iter = this->first_set[rj].begin(); value_iter != this->first_set[rj].end(); value_iter++)
				{
					if (*value_iter != '$')
						select.insert(*value_iter);
				}
				if (this->first_set[rj].find('$') != this->first_set[rj].end())
				{
					countEmpty++;
				}
				else
				{
					break;
				}
			}
		}

		// 将Follow集合并入
		if (countEmpty == this->P[i].right.length())
		{
			set<char>::iterator value_iter;
			for (value_iter = this->follow_set[this->P[i].left].begin(); value_iter != this->follow_set[this->P[i].left].end(); value_iter++)
			{
				select.insert(*value_iter);
			}
		}

		this->select_set.push_back(select);
	}
}

// 生成预测分析表，当同一单元格有多个式子时，返回false
bool Grammar::get_Table()
{
	int i, j;

	#pragma region 初始化预测分析表
	this->predict_table = new int* [this->nonterminal.size()];
	for (i = 0; i < this->nonterminal.size(); i++)
	{
		this->predict_table[i] = new int[this->terminal.size()];
	}
	for (i = 0; i < this->nonterminal.size(); i++)
	{
		for (j = 0; j < this->terminal.size(); j++)
		{
			this->predict_table[i][j] = 0;
		}
	}
	#pragma endregion

	for (i = 0; i < this->pnum; i++)
	{
		int row = index_in_nonterminal(this->P[i].left);
		set<char>::iterator s_iter;
		for (s_iter = this->select_set[i].begin(); s_iter != this->select_set[i].end(); s_iter++)
		{
			int column = index_in_terminal(*s_iter);
			if (this->predict_table[row][column] != 0)
			{
				return false;
			}
			else
			{
				this->predict_table[row][column] = i;
			}
		}
	}
	return true;

}

// 判断是否为LL(1)文法
bool Grammar::is_LL1()
{
	if (!get_Table())
		return false;
	return true;
}

// 求target的First集合
void Grammar::get_First_recur(char target)
{
	int i, j;
	int isEmpty = 0;		// 产生空串标识符，0表示不能产生$
	int countEmpty = 0;		// 只有当X->Y1Y2...Yn中，Y1-Yn都可以产生空串时，First(X)才有#
	for (i = 0; i < this->pnum; i++)
	{
		// 与产生式左端匹配
		if (this->P[i].left == target)
		{
			// 终结符直接加入First
			if (inVt(this->P[i].right[0]))
			{
				this->first_set[target].insert(this->P[i].right[0]);
			}
			else
			{
				// X->Y1Y2...Yj...Yk这样的表达式
				for (j = 0; j < this->P[i].right.length(); j++)
				{
					char Yj = this->P[i].right[j];
					// 如果Yj是终结符，则停止递归
					if (inVt(Yj))
					{
						this->first_set[target].insert(Yj);
						break;
					}
					// Yj是非终结符则应递归，先求出Yj的First集
					get_First_recur(Yj);

					// 将Yj的结果复制给X
					set<char>::iterator value_iter;
					for (value_iter = this->first_set[Yj].begin(); value_iter != this->first_set[Yj].end(); value_iter++)
					{
						if (*value_iter == '$')
						{
							isEmpty = 1;
						}
						else
						{
							this->first_set[target].insert(*value_iter);
						}
					}

					if (isEmpty == 0)	// Yj不能产生空，迭代结束
					{
						break;
					}
					else       // 如果能产生空串，那么需要确认右侧全都能产生空串
					{
						countEmpty += 1;
						isEmpty = 0;
					}
				}
				if (countEmpty == this->P[i].right.length())
				{
					this->first_set[target].insert('$');
				}
			}
		}
	}
}

// 递归的求target的Follow集合
void Grammar::get_Follow_recur(char target,int recur_count)
{
	int i, j;
	for (i = 0; i < this->pnum; i++)
	{
		// 限制最大递归次数，防止右递归产生式求Follow时死循环
		if (recur_count > this->pnum)
		{
			break;
		}
		int index = this->P[i].right.find(target);	// 找到target在产生式中P[i]右端的下标

		// !npos表示找到target，下面对形如S->aAB这样的产生式(A不为最右)
		if (index != string::npos && index < this->P[i].right.length() - 1)
		{
			char next = this->P[i].right[index + 1];
			// 如果是终结符直接加入
			if (inVt(next))
			{
				this->follow_set[target].insert(next);
			}
			else
			{
				int hasEmpty = 0;	// 含有终结符标识
				set<char>::iterator next_iter;
				for (next_iter = this->first_set[next].begin(); next_iter != this->first_set[next].end(); next_iter++)
				{
					if (*next_iter == '$')
						hasEmpty = 1;
					else
					{
						this->follow_set[target].insert(*next_iter);
					}
				}

				// 若为 S->aABC，当First(B)含有$时，首先做Follow(A)+= First(B)-$，即上面做的部分；然后做Follow(A)+=Follow(B)
				if (hasEmpty == 1 && ((index + 1) < this->P[i].right.length() - 1))
				{
					get_Follow_recur(next, ++recur_count);
					set<char>::iterator next_iter;
					for (next_iter = this->follow_set[next].begin(); next_iter != this->follow_set[next].end(); next_iter++)
					{
						this->follow_set[target].insert(*next_iter);
					}
				}

				// 仅对 S->aAB，当First(B)含有$时，Follow(A)+=Follow(S)。即target后面的非终结符为最右端
				// 特别的，对于 S->aSA，First(A)含有$，需要避免无限递归
				if (hasEmpty == 1 && ((index + 1) == this->P[i].right.length() - 1) && this->P[i].left != target)
				{
					get_Follow_recur(this->P[i].left, ++recur_count);
					set<char>::iterator left_iter;
					for (left_iter = this->follow_set[this->P[i].left].begin(); left_iter != this->follow_set[this->P[i].left].end(); left_iter++)
					{
						this->follow_set[target].insert(*left_iter);
					}
				}
			}
		}
		// 对于形如 S->aA，则Follow(A)+=Follow(S)
		else if (index != string::npos && index == this->P[i].right.length() - 1 && target != this->P[i].left)
		{
			get_Follow_recur(this->P[i].left, ++recur_count);
			set<char>::iterator left_iter;
			for (left_iter = this->follow_set[this->P[i].left].begin(); left_iter != this->follow_set[this->P[i].left].end(); left_iter++)
			{
				this->follow_set[target].insert(*left_iter);
			}
		}
	}
}

// 非递归的求target的Follow集合
void Grammar::get_Follow()
{
	int i, j;
	int flag = 1;		// 当所有Follow集合不再变化时，停止循环

	while (flag)
	{
		flag = 0;		// 设flag为0，当有修改时设为1
		
		set<char>::iterator vn_iter;
		// 对每个非终结符
		// 对每个产生式
		for (i = 0; i < this->pnum; i++)
		{
			char left = this->P[i].left;
			for (j = 0; j < this->P[i].right.length(); j++)
			{
				char rj = this->P[i].right[j];
				if (inVn(rj))
				{
					char brj;	// behind rj
					int psize = this->follow_set[rj].size();
					if (j < this->P[i].right.length() - 1)	// Follow(rj)+=First(brj)
					{
						brj = this->P[i].right[j + 1];
						if (inVt(brj))
						{
							this->follow_set[rj].insert(brj);
						}
						else
						{
							int emptyFlag = 0;
							set<char>::iterator brj_iter;
							for (brj_iter = this->first_set[brj].begin(); brj_iter != this->first_set[brj].end(); brj_iter++)
							{
								if (*brj_iter != '$')
									this->follow_set[rj].insert(*brj_iter);
								else
									emptyFlag = 1;
							}

							if (emptyFlag == 1)		// A->aBCD，C若能推出ε，Follow(B)+=Follow(C)
							{
								for (brj_iter = this->follow_set[brj].begin(); brj_iter != this->follow_set[brj].end(); brj_iter++)
								{
									this->follow_set[rj].insert(*brj_iter);
								}
							}
						}
					}
					else    // Follow(rj)+=Follow(left)
					{
						set<char>::iterator left_iter;
						for (left_iter = this->follow_set[left].begin(); left_iter != this->follow_set[left].end(); left_iter++)
						{
							this->follow_set[rj].insert(*left_iter);
						}
					}
					if (psize != this->follow_set[rj].size())
						flag = 1;
				}
			}
		}
	}
}

// 判断字符是否为非终结符
bool Grammar::inVn(char s)
{
	if (this->Vn.find(s) != this->Vn.end())
		return true;
	return false;
}

// 判断字符是否为终结符
bool Grammar::inVt(char e)
{
	if (this->Vt.find(e) != this->Vt.end())
		return true;
	return false;
}

// 求终结符在预测分析表中的列标
int Grammar::index_in_terminal(char target)
{
	int i;
	set<char>::iterator t_iter;
	for (t_iter = this->terminal.begin(),i=0; t_iter != this->terminal.end(); t_iter++,i++)
	{
		if (*t_iter == target)
			return i;
	}
	return 0;
}

// 求非终结符在预测分析表中的行标
int Grammar::index_in_nonterminal(char target)
{
	int i;
	set<char>::iterator nt_iter;
	for (nt_iter = this->nonterminal.begin(), i = 0; nt_iter != this->nonterminal.end(); nt_iter++, i++)
	{
		if (*nt_iter == target)
			return i;
	}
	return 0;
}

// 输出产生式集
void Grammar::printProduction()
{
	int i;
	cout << "--------------产生式集合---------------" << endl << endl;
	for (i = 0; i < this->pnum; i++)
	{
		cout << this->P[i].left << "->" << this->P[i].right << endl;
	}
	cout << endl;
}

// 输出First集合
void Grammar::printFirst()
{
	cout << "-----------First集合-------------" << endl << endl;
	map<char, set<char>>::iterator first_iter;
	for (first_iter = this->first_set.begin(); first_iter != this->first_set.end(); first_iter++)
	{
		cout << "非终结符" << first_iter->first << ":";
		set<char>::iterator value_iter;
		for (value_iter = first_iter->second.begin(); value_iter != first_iter->second.end(); value_iter++)
		{
			cout << *value_iter << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// 输出Follow集合
void Grammar::printFollow()
{
	cout << "-----------Follow集合-------------" << endl << endl;
	map<char, set<char>>::iterator follow_iter;
	for (follow_iter = this->follow_set.begin(); follow_iter != this->follow_set.end(); follow_iter++)
	{
		cout << "非终结符" << follow_iter->first << ":";
		set<char>::iterator value_iter;
		for (value_iter = follow_iter->second.begin(); value_iter != follow_iter->second.end(); value_iter++)
		{
			cout << *value_iter << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// 输出Select集合
void Grammar::printSelect()
{
	cout << "------------Select集合------------" << endl << endl;
	int i;
	for (i = 0; i < this->pnum; i++)
	{
		cout << this->P[i].left << "->" << this->P[i].right << ":\t";
		set<char>::iterator iter;
		for (iter = this->select_set[i].begin(); iter != this->select_set[i].end(); iter++)
		{
			cout << *iter << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// 输出预测分析表
void Grammar::printTable()
{
	int i, j;
	cout << "-------------预测分析表---------------" << endl << endl;
	set<char>::iterator it;
	for (it = this->terminal.begin(); it != this->terminal.end(); it++)
	{
		cout << "\t" << *it;
	}
	cout << endl;
	for (it = this->nonterminal.begin(), i = 0; it != this->nonterminal.end(); it++, i++)
	{
		cout << *it;
		for (j = 0; j < this->terminal.size(); j++)
		{
			if (this->predict_table[i][j] != 0)
			{
				cout << "\t";
				cout << this->P[this->predict_table[i][j]].left << "->" << this->P[this->predict_table[i][j]].right;
			}
			else
			{
				cout << "\t";
			}
		}
		cout << endl;
	}
	cout << endl;
}

Grammar::~Grammar()
{
	this->Vn.clear();
	this->Vt.clear();
	this->P.clear();
	this->terminal.clear();
	this->nonterminal.clear();
	this->first_set.clear();
	this->follow_set.clear();
	this->select_set.clear();
	delete[] this->predict_table;
}

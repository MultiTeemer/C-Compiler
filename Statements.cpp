#include <iostream>
#include "Statements.h"

static const int M = 2;

void Block::print(int deep) const
{
	if (locals && locals->size() > 0)
	{
		cout << string(deep * M, ' ') << "Locals:" << endl;
		locals->print(deep + 1);
		cout << endl;
	}
	if (statements.size() > 0)
	{
		cout << string(deep * M, ' ') << "Instructions:" << endl;
		for (int i = 0; i < statements.size(); i++)
			statements[i]->print(deep + 1);
		cout << endl;
	}
	if (statements.size() == 0 && locals->size() == 0)
		cout << string(deep * M, ' ') << "<empty block>" << endl << endl;	
}

void CycleStatement::print(int deep) const
{
	string tab(deep * M, ' ');
	cout << tab << "Condition:" << endl;
	condition->print(deep + 1);
	cout << tab << "Block:" << endl;
	body->print(deep + 1);
}

void WhilePreCondStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "while () do .." << endl;
	CycleStatement::print(deep);
}

void WhilePostCondStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "do .. while ()" << endl;
	CycleStatement::print(deep);
}

void ForStatement::print(int deep) const
{
	string tab(deep * M, ' ');
	cout << tab << "for (;;) .." << endl;
	cout << tab << "First action:" << endl;
	if (initialization)
		initialization->print(deep + 1);
	cout << tab << "Condition:" << endl;
	if (condition)
		condition->print(deep + 1);
	cout << tab << "Step:" << endl;
	if (increment)
		increment->print(deep + 1);
	cout << tab << "Block:" << endl;
	if (body)
		body->print(deep + 1);
	else
		cout << tab << "<empty block>";
}

void IfStatement::print(int deep) const
{
	string tab(deep * M, ' ');
	cout << tab << "if" << endl << endl;
	condition->print(deep + 1);
	cout << endl << tab << "then" << endl << endl;
	trueBranch->print(deep + 1);
	if (falseBranch)
	{
		cout << tab << "else" << endl << endl;
		falseBranch->print(deep + 1);
	}
}

void ContinueStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "continue" << endl;
}

void BreakStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "break" << endl;
}

void ReturnStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "return" << endl;
	if (arg)
		arg->print(deep + 1);
}
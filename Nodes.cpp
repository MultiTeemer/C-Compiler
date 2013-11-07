#include <iostream>
#include "Nodes.h"
#include "Exceptions.h"

using namespace std;

static const int M = 2;

string OpNode::opName() const
{
	char operations[][5] = { "+", "-", "*", "/", "%", "++", "--", "+=", "-=", "*=", "/=", "%=", "?", ":",
		"&", "|", "&&", "||", "!", "^", "&=", "|=", "^=", "=", "==", "!=", ">", "<", ">=", "<=", "<<", ">>", ".", "->",
		"<<=", ">>=", "(", ")", "[", "]", ",", "~" };
	return string(operations[dynamic_cast<OpToken*>(token)->val]);
}

BinaryOpNode::BinaryOpNode(Token* op, Node* l, Node* r): OpNode(op), left(l), right(r) 
{
	if (l == 0 || r == 0)
		throw ParserException("Lost operand", op->line, op->col);
}

void BinaryOpNode::print(int deep) const
{
	left->print(deep + 1);
	cout << string(deep * M, ' ') << opName() << endl;
	right->print(deep + 1);
}

void IntNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<IntegerToken*>(token)->val << endl;
}

void FloatNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<FloatToken*>(token)->val << endl;
}

void IdentifierNode::print(int deep) const
{
	cout << string(deep * M, ' ') << sym->name << endl;
}

UnaryOpNode::UnaryOpNode(Token* op, Node* oper): OpNode(op), operand(oper) 
{
	if (!oper)
		throw ScannerException("Lost operand", op->line, op->col);
}

void UnaryOpNode::print(int deep) const
{
	cout << string(deep * M, ' ') << opName() << endl;
	operand->print(deep + 1);
}

void PostfixUnaryOpNode::print(int deep) const
{
	operand->print(deep);
	cout << string(deep * M, ' ') << opName() << endl;
}

void CoerceNode::print(int deep) const
{
	cout << string(deep * M, ' ') << type->name;
	cout << string(deep * M, ' ') << "(" << endl;
	operand->print(deep + 1);
	cout << string(deep * M, ' ') << ")" << endl;
}

void FunctionalNode::printArgs(int deep) const
{
	for (int i = 0; i < args.size(); i++)
	{
		args[i]->print(deep + 1);
		if (i < args.size() - 1)
			cout << string(deep * M, ' ') << ',' << endl;
	}
}

void FuncCallNode::print(int deep) const
{
	name->print(deep);
	cout << string(deep * M, ' ') << "(" << endl;
	printArgs(deep);
	cout << string(deep * M, ' ') << ")" << endl;
}

void ArrNode::print(int deep) const 
{
	name->print(deep);
	cout << string(deep * M, ' ') << "[" << endl;
	printArgs(deep);
	cout << string(deep * M, ' ') << "]" << endl;
}

string KeywordNode::KeywordName() const
{
	switch (dynamic_cast<KeywordToken*>(token)->val)
	{
	case CHAR:
		return string("char");
	case INT:
		return string("int");
	case FLOAT:
		return string("float");
	case SIZEOF:
		return string("sizeof");
	default:
		throw ParserException("unknown function", token->line, token->col);
	}
}

void KeywordNode::print(int deep) const
{
	cout << string(deep * M, ' ') << KeywordName() << endl;
}

void CharNode::print(int deep) const
{
	cout << string(deep * M, ' ') << '\'' << dynamic_cast<CharToken*>(token)->val << '\'' << endl; 
}

void StringNode::print(int deep) const 
{ 
	cout << string(deep * M, ' ') << '"' << dynamic_cast<StringToken*>(token)->val << '"' << endl; 
}

void TernaryOpNode::print(int deep) const
{
	string tab = string(deep * M, ' ');
	cout << tab << "(" << endl;
	condition->print(deep + 1);
	cout << tab << ")" << endl;
	cout << tab << "?" << endl;
	left->print(deep + 1);
	cout << tab << ":" << endl;
	right->print(deep + 1);
}

void Block::print(int deep, Block::OutputModsT mode) const
{
	if (mode == DECLARATIONS && locals)
		locals->print(deep + 1);
	else if (mode == STATEMENTS)
		for (int i = 0; i < statements.size(); i++)
			statements[i]->print(deep + 1);
	else {
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
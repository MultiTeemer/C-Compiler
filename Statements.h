#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "Nodes.h"

class Expression
{
private:
	Node* root;
public:
	friend class Parser;
	Expression(Node* r): root(r) {}
	void print(int deep) const { root->print(); }
};

class Statement 
{
public:
	virtual void print(int deep) const = 0;
};

class SymTable;

class Block : public Statement
{
private:
	SymTable* locals;
	vector<Statement*> statements;
public:
	friend class Parser;
	Block(SymTable* l): locals(l), statements(0) {}
	void AddStatement(Statement* stmt) { statements.push_back(stmt); }
	void print(int deep) const; 
};

class SingleStatement : public Statement
{
private:
	Expression* expr;
public:
	SingleStatement(Expression* e): expr(e) {}
	void print(int deep) const { expr->print(deep); }
};

class CondStatement : public Statement
{
protected:
	Expression* condition;
public:
	CondStatement(Expression* cond): condition(cond) {}
};

class IfStatement : public CondStatement
{
private:
	Statement* trueBranch;
	Statement* falseBranch;
public:
	friend class Parser;
	IfStatement(Expression* cond, Statement* tB, Statement* fB): CondStatement(cond), trueBranch(tB), falseBranch(fB) {}
	void print(int deep) const;
};

class CycleStatement : public CondStatement
{
protected:
	Statement* body;
public:
	CycleStatement(Expression* cond, Statement* b): CondStatement(cond), body(b) {}
	virtual void print(int deep) const;
};

class WhilePreCondStatement : public CycleStatement
{
public:
	friend class Parser;
	WhilePreCondStatement(Expression* cond, Statement* b): CycleStatement(cond, b) {}
	void print(int deep) const;
};

class WhilePostCondStatement : public CycleStatement
{
public:
	friend class Parser;
	WhilePostCondStatement(Expression* cond, Statement* b): CycleStatement(cond, b) {}
	void print(int deep) const;
};

class ForStatement : public CycleStatement
{
private:
	Expression* initialization;
	Expression* increment;
public:
	friend class Parser;
	ForStatement(Expression* condition, Expression* inc, Expression* initial, Statement* block): 
		CycleStatement(condition, block), initialization(initial), increment(inc) {}
	void print(int deep) const;
};

class JumpStatement : public Statement
{};

class BreakStatement : public JumpStatement
{
public:
	void print(int deep) const;
};

class ContinueStatement : public JumpStatement
{
public:
	void print(int deep) const;
};

class ReturnStatement : public JumpStatement
{
private:
	Expression* arg;
public:
	ReturnStatement(Expression* a): arg(a) {}
	void print(int deep) const;
};

#endif
#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <string>
#include <vector>
#include <map>
#include "Nodes.h"

using namespace std;

class SymTableStack : public SymInterface
{
private:
	vector<SymTable*> tables;	
public:
	Symbol* find(const string& name) const;
	SymTable* top();
	bool existsInLastNamespace(const string& name);	
	void add(Symbol* s);
	void print(int deep = 0) const;
	void push(SymTable* table);
	void pop();
};

class SingleStatement : public Statement
{
private:
	Node* expr;
public:
	SingleStatement(Node* e): expr(e) {}
	void print(int deep) const { expr->print(deep); }
	void generate(AsmCode& code) const;
};

class CondStatement : public Statement
{
protected:
	Node* condition;
public:
	CondStatement(Node* cond): condition(cond) {}
};

class IfStatement : public CondStatement
{
private:
	Statement* trueBranch;
	Statement* falseBranch;
public:
	friend class Parser;
	IfStatement(Node* cond, Statement* tB, Statement* fB): CondStatement(cond), trueBranch(tB), falseBranch(fB) {}
	void print(int deep) const;
	void generate(AsmCode& code) const;
};

class CycleStatement : public CondStatement
{
protected:
	Statement* body;
public:
	CycleStatement(Node* cond, Statement* b): CondStatement(cond), body(b) {}
	virtual void print(int deep) const;
};

class WhilePreCondStatement : public CycleStatement
{
public:
	friend class Parser;
	WhilePreCondStatement(Node* cond, Statement* b): CycleStatement(cond, b) {}
	void print(int deep) const;
	void generate(AsmCode& code) const;
};

class WhilePostCondStatement : public CycleStatement
{
public:
	friend class Parser;
	WhilePostCondStatement(Node* cond, Statement* b): CycleStatement(cond, b) {}
	void print(int deep) const;
	void generate(AsmCode& code) const;
};

class ForStatement : public CycleStatement
{
private:
	Node* initialization;
	Node* increment;
public:
	friend class Parser;
	ForStatement(Node* initial, Node* condition, Node* inc, Statement* block): 
		CycleStatement(condition, block), initialization(initial), increment(inc) {}
	void print(int deep) const;
	void generate(AsmCode& code) const;
};

class JumpStatement : public Statement
{};

class BreakStatement : public JumpStatement
{
public:
	void print(int deep) const;
	void generate(AsmCode& code) const;
};

class ContinueStatement : public JumpStatement
{
public:
	void print(int deep) const;
	void generate(AsmCode& code) const;
};

class ReturnStatement : public JumpStatement
{
private:
	Node* arg;
	FuncSym* owner;
public:
	ReturnStatement(Node* a, FuncSym* o): arg(a), owner(o) {}
	void print(int deep) const;
	void generate(AsmCode& code) const;
};


#endif
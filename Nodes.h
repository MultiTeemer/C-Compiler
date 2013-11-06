#ifndef NODES_H
#define NODES_H

#include <string>
#include <vector>
#include "Tokens.h"
#include "Symbols.h"

using namespace std;

class Node
{
protected:
	Token* token;
public:
	friend class Parser;
	Node(): token(0) {}
	Node(Token* t): token(t) {}
	virtual void print(int deep = 0) const = 0;
};

class EmptyNode : public Node
{
public:
	EmptyNode(): Node(0) {}
	void print(int deep) const {}
};

class OpNode : public Node
{
protected:
	string opName() const;
public:
	OpNode(Token* op): Node(op) {}
	virtual void print(int deep) {}
};

class BinaryOpNode : public OpNode
{
protected:
	Node* left;
	Node* right;
public:	
	friend class Parser;
	BinaryOpNode(Token* op, Node* l, Node* r);
	void print(int deep) const;
};

class UnaryOpNode : public OpNode
{
protected:
	Node* operand;
public:
	UnaryOpNode(Token* op, Node* oper);
	void print(int deep) const;
};

class PostfixUnaryOpNode : public UnaryOpNode
{
public:
	PostfixUnaryOpNode(Token* op, Node* oper): UnaryOpNode(op, oper) {}
	void print(int deep) const;
};

class CoerceNode : public UnaryOpNode
{
private:
	Symbol* type;
public:
	CoerceNode(Token* op, Node* oper, Symbol* ts): UnaryOpNode(op, oper), type(ts) {}
	void print(int deep) const;
};

class TernaryOpNode : public BinaryOpNode
{
private:
	Node* condition;
public:
	TernaryOpNode(Token* op, Node* c, Node* l, Node* r): BinaryOpNode(op, l, r), condition(c) {}
	void print(int deep) const;
};

class IntNode : public Node
{
public:	
	IntNode(Token* t): Node(t) {}
	void print(int deep) const;
};

class FloatNode : public Node
{
public:	
	FloatNode(Token* t): Node(t) {}
	void print(int deep) const;
};

class IdentifierNode : public Node
{
private:
	Symbol* sym;
public:	
	IdentifierNode(Symbol* s): Node(0), sym(s) {}
	void print(int deep) const;
};

class FunctionalNode : public Node
{
protected:
	Node* name;
	vector<Node*> args;
	void printArgs(int deep) const;
public:
	FunctionalNode(Node* n): name(n), args(0) {}
	void addArg(Node* arg) { args.push_back(arg); }
};

class FuncCallNode : public FunctionalNode
{
private:
	Symbol* symbol;
public:
	FuncCallNode(Node* func, Symbol* funcsym): FunctionalNode(func), symbol(funcsym) {}
	void print(int deep) const;
};

class ArrNode : public FunctionalNode
{
public:
	ArrNode(Node* arr): FunctionalNode(arr) {}
	void print(int deep) const;
};

class KeywordNode : public Node
{
private:
	string KeywordName() const;
public:
	KeywordNode(Token* t): Node(t) {}
	void print(int deep) const;
};

class CharNode : public Node
{
public:
	CharNode(Token* t): Node(t) {}
	void print(int deep) const;
};

class StringNode : public Node
{
public:
	StringNode(Token* t): Node(t) {}
	void print(int deep) const;
};

typedef Node* NodeP;

class BlockNode : public Node 
{
private:
	Block* block;
public:
	BlockNode(Block* b): block(b) {}
	void print(int deep) const { block->print(deep + 1); }
};

class Statement : public Node 
{};

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
	Node* trueBranch;
	Node* falseBranch;
public:
	friend class Parser;
	IfStatement(Node* cond, Node* tB, Node* fB): CondStatement(cond), trueBranch(tB), falseBranch(fB) {}
	void print(int deep) const;
};

class CycleStatement : public CondStatement
{
protected:
	Node* body;
public:
	CycleStatement(Node* cond, Node* b): CondStatement(cond), body(b) {}
	virtual void print(int deep) const;
};

class WhilePreCondStatement : public CycleStatement
{
public:
	friend class Parser;
	WhilePreCondStatement(Node* cond, Node* b): CycleStatement(cond, b) {}
	void print(int deep) const;
};

class WhilePostCondStatement : public CycleStatement
{
public:
	friend class Parser;
	WhilePostCondStatement(Node* cond, Node* b): CycleStatement(cond, b) {}
	void print(int deep) const;
};

class ForStatement : public CycleStatement
{
private:
	Node* initialization;
	Node* increment;
public:
	friend class Parser;
	ForStatement(Node* condition, Node* block, Node* initial, Node* inc): CycleStatement(condition, block), initialization(initial), increment(inc) {}
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
	Node* arg;
public:
	ReturnStatement(Node* a): arg(a) {}
	void print(int deep) const;
};

#endif
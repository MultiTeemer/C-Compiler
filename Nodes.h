#ifndef NODES_H
#define NODES_H

#include <string>
#include <vector>
#include "Tokens.h"
#include "BaseSymbols.h"
#include "Exceptions.h"

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
	virtual TypeSym* getType() const { return 0; }
	static Node* makeTypeCoerce(Node* expr, TypeSym* from, TypeSym* to);
};

class EmptyNode : public Node
{
public:
	EmptyNode(): Node(0) {}
	void print(int deep) const;
};

class OpNode : public Node
{
protected:
	string opName() const;
public:
	OpNode(Token* op): Node(op) {}
	virtual void print(int deep) {}
};

class UnaryOpNode : public OpNode
{
protected:
	Node* operand;
public:
	UnaryOpNode(Token* op, Node* oper);
	void print(int deep) const;
	virtual TypeSym* getType() const;
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
	TypeSym* type;
public:
	CoerceNode(Token* op, Node* oper, TypeSym* ts): UnaryOpNode(op, oper), type(ts) {}
	void print(int deep) const;
	virtual TypeSym* getType() const;
};

class BinaryOpNode : public OpNode
{
protected:
	mutable Node* left;
	mutable	Node* right;
public:	
	friend class Parser;
	BinaryOpNode(Token* op, Node* l, Node* r);
	void print(int deep) const;
	virtual TypeSym* getType() const;
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
	virtual TypeSym* getType() const;
};

class FloatNode : public Node
{
public:	
	FloatNode(Token* t): Node(t) {}
	void print(int deep) const;
	virtual TypeSym* getType() const;
};

class IdentifierNode : public Node
{
public:		
	Symbol* sym;
	IdentifierNode(Token* t, Symbol* s): Node(t), sym(s) {}
	void print(int deep) const;
	virtual TypeSym* getType() const;
};

class FunctionalNode : public Node
{
protected:
	Node* name;
	mutable vector<Node*> args;
	void printArgs(int deep) const;
public:
	FunctionalNode(Token* tok, Node* n): Node(tok), name(n), args(0) {}
	void addArg(Node* arg) { args.push_back(arg); }
};

class FuncCallNode : public FunctionalNode
{
private:
	Symbol* symbol;
public:
	FuncCallNode(Token* t, Node* func, Symbol* funcsym): FunctionalNode(t, func), symbol(funcsym) {}
	void print(int deep) const;
	virtual TypeSym* getType() const;
};

class ArrNode : public FunctionalNode
{
public:
	ArrNode(Token* t, Node* arr): FunctionalNode(t, arr) {}
	void print(int deep) const;
	virtual TypeSym* getType() const;
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
	virtual TypeSym* getType() const;
};

class StringNode : public Node
{
public:
	StringNode(Token* t): Node(t) {}
	void print(int deep) const;
	virtual TypeSym* getType() const;
};

typedef Node* NodeP;

#endif
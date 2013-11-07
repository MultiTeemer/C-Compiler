#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class Symbol;

class SymInterface
{
public:
	virtual Symbol* find(const string& name) const = 0;
	virtual void add(Symbol* s) = 0;
	virtual void print(int deep) const = 0;
};

class SymTable : public SymInterface
{
private:
	vector<string> names;
	vector<Symbol*> symbols;
	map<string, int> index;
public:
	friend class FuncSym;
	SymTable(): names(0), symbols(0) {}
	Symbol* find(const string& name) const;
	void add(Symbol* s);
	void print(int deep = 0) const;
	bool exists(const string& name);
	int size() const;
};

class SymTableStack : public SymInterface
{
private:
	vector<SymTable*> tables;	
public:
	Symbol* find(const string& name) const;
	SymTable* top();
	void add(Symbol* s);
	void print(int deep = 0) const;
	bool existsInLastNamespace(const string& name);	
	void push(SymTable* table);
	void pop();
};

class Symbol
{
public:
	string name;
	Symbol(const string& n): name(n) {}
	virtual void print(int deep) const;
};

class TypeSym : public Symbol
{
public:
	TypeSym(const string& n): Symbol(n) {}
	virtual string typeName() const;
	bool operator == (const string& o) const;
	virtual bool isStruct() { return false; }
	virtual TypeSym* nextType() const { return 0; }
	virtual void setNextType(TypeSym* type) {}
};

class ConstTypeSym : public TypeSym
{
private:
	TypeSym* type;
public:
	ConstTypeSym(TypeSym* t): TypeSym(""), type(t) {}
	string typeName() const;
	bool isStruct() { return type->isStruct(); }
};

class ScalarSym : public TypeSym
{
public:
	ScalarSym(const string& n): TypeSym(n) {}
};

class ArraySym : public TypeSym
{
private:
	int size;
	TypeSym* type;
public:
	friend class Parser;
	ArraySym(TypeSym* t, int s): TypeSym(""), type(t), size(s) {}
	string typeName() const;
	TypeSym* nextType() const { return type; }
	void setNextType(TypeSym* t) { type = t; }
};

class PointerSym : public TypeSym 
{
private:
	TypeSym* type;
public:
	PointerSym(TypeSym* t): TypeSym(""), type(t) {}
	string typeName() const;
	TypeSym* nextType() const { return type; }
	void setNextType(TypeSym* t) { type = t; }
};

class AliasSym : public TypeSym
{
private:
	TypeSym* type;
public:
	AliasSym(const string& name, TypeSym* t): TypeSym(name), type(t) {}
	void print(int deep) const;
	string typeName() const;
};

class StructSym : public TypeSym
{
private:
	SymTable* fields;
public:
	friend class Parser;
	StructSym(const string& name, SymTable* f): TypeSym(name), fields(f) {}
	void print(int deep) const;
	bool isStruct() { return true; }
	string typeName() const;
};

class VarSym : public Symbol
{
private:
	TypeSym* type;
public:
	friend class Parser;
	friend class FuncSym;
	VarSym(const string& n, TypeSym* t): Symbol(n), type(t) {} 
	void print(int deep) const;
};

class Node;

class Block
{
private:
	SymTable* locals;
	vector<Node*> statements;
public:
	typedef enum {
		DECLARATIONS,
		STATEMENTS,
		ALL
	} OutputModsT;
	friend class Parser;
	Block(SymTable* l): locals(l), statements(0) {}
	void AddStatement(Node* stmt) { statements.push_back(stmt); }
	void print(int deep, OutputModsT mode = ALL) const; 
};

class FuncSym : public TypeSym
{
private:
	SymTable* params;
	Block* body;
	TypeSym* val;
public:
	friend class Parser;
	FuncSym(const string& n, TypeSym* v): TypeSym(n), val(v), params(0), body(0) {} 
	string typeName() const;
	void print(int deep) const;
	TypeSym* nextType() const { return val; }
	void setNextType(TypeSym* t) { val = t; }
};

#endif
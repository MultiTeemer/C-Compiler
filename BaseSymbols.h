#ifndef BASE_SYMBOLS_H
#define BASE_SYMBOLS_H

#include <string>
#include <map>

using namespace std;

class TypeSym;

class Symbol
{
public:
	string name;
	Symbol(const string& n): name(n) {}
	virtual void print(int deep) const;
	virtual TypeSym* getType() { return 0; }
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
	virtual TypeSym* getType() { return this; }
};

class ScalarSym : public TypeSym
{
public:
	ScalarSym(const string& n): TypeSym(n) {}
};

class PointerSym : public TypeSym 
{
public:	
	TypeSym* type;
	PointerSym(TypeSym* t): TypeSym(""), type(t) {}
	string typeName() const;
	TypeSym* nextType() const { return type; }
	void setNextType(TypeSym* t) { type = t; }
};

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
	friend class FuncCallNode;
	SymTable(): names(0), symbols(0) {}
	Symbol* find(const string& name) const;
	void add(Symbol* s);
	void print(int deep = 0) const;
	bool exists(const string& name);
	int size() const;
};

class Statement 
{
public:
	virtual void print(int deep) const = 0;
};

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

class FuncSym : public TypeSym
{
private:
	SymTable* params;
	Block* body;
	TypeSym* val;
public:
	friend class Parser;
	friend class FuncCallNode;
	FuncSym(const string& n, TypeSym* v): TypeSym(n), val(v), params(0), body(0) {} 
	string typeName() const;
	void print(int deep) const;
	TypeSym* nextType() const { return val; }
	void setNextType(TypeSym* t) { val = t; }
	TypeSym* getType() { return val; } 
};

extern ScalarSym* intType;
extern ScalarSym* floatType;
extern ScalarSym* charType;
extern ScalarSym* voidType;
extern map<TypeSym*, int> typePriority;
extern map<OperationsT, TypeSym*> operationTypeOperands;
extern map<OperationsT, TypeSym*> operationReturningType;

#endif
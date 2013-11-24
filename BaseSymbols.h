#ifndef BASE_SYMBOLS_H
#define BASE_SYMBOLS_H

#include <string>
#include <map>
#include "Commands.h"

using namespace std;

class TypeSym;

class Symbol
{
public:
	string name;
	Symbol(const string& n): name(n) {}	
	virtual int byteSize() const { return 0; }
	virtual TypeSym* getType() { return 0; }
	virtual void print(int deep) const;
	virtual void generate(AsmCode& code) const {}
};

class TypeSym : public Symbol
{
public:
	TypeSym(const string& n): Symbol(n) {}
	virtual string typeName() const;
	bool operator == (const string& o) const;
	virtual TypeSym* nextType() const { return 0; }
	virtual TypeSym* getType() { return this; }	
	virtual bool isStruct() { return false; }
	virtual bool isLvalue() const { return false; }
	virtual bool isModifiableLvalue() const { return false; }
	virtual bool canConvertTo(TypeSym* to) { return false; }
	virtual bool operator == (TypeSym* o) const { return this == o; }	
	virtual void setNextType(TypeSym* type) {}
	bool operator != (TypeSym* o) const { return !(*this == o); }
};

class ScalarSym : public TypeSym
{
public:
	ScalarSym(const string& n): TypeSym(n) {}
	bool canConvertTo(TypeSym* to);
	bool isLvalue() const { return true; }
	bool isModifiableLvalue() const { return true; }
	int byteSize() const { return name == "char" ? 1 : 4; }
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
	bool isLvalue() const { return true; }
	bool operator == (TypeSym* t) const;
	bool canConvertTo(TypeSym* t);
	void generate(AsmCode& code) const;
	int byteSize() const { return type->byteSize() * size; }
};

class PointerSym : public TypeSym 
{
public:	
	TypeSym* type;
	PointerSym(TypeSym* t): TypeSym(""), type(t) {}
	string typeName() const;
	TypeSym* nextType() const { return type; }
	void setNextType(TypeSym* t) { type = t; }
	bool isModifiableLvalue() const { return true; }
	bool isLvalue() const { return true; }
	bool canConvertTo(TypeSym* to);
	bool operator == (TypeSym* o) const;
	int byteSize() const { return 4; }
};

class VarSym : public Symbol
{
public:	
	TypeSym* type;
	VarSym(const string& n, TypeSym* t): Symbol(n), type(t) {} 
	void print(int deep) const;
	TypeSym* getType() { return type; }
	void generate(AsmCode& code) const;
	int byteSize() const { return type->byteSize(); }
};

class ConstTypeSym : public TypeSym
{
private:
	TypeSym* type;
public:
	ConstTypeSym(TypeSym* t): TypeSym(""), type(t) {}
	string typeName() const;
	bool isStruct() { return type->isStruct(); }
	int byteSize() const { return type->byteSize(); }
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
	void generate(AsmCode& code) const;	
	bool exists(const string& name);
	bool operator == (SymTable* o) const;
	int size() const;
};

class Statement 
{
public:
	virtual void print(int deep) const = 0;
	virtual void generate(AsmCode& code) const = 0;
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
	void generate(AsmCode& code) const;
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
	FuncSym(TypeSym* v): TypeSym(""), val(v), params(0), body(0) {} 
	string typeName() const;
	void print(int deep) const;
	TypeSym* nextType() const { return val; }
	void setNextType(TypeSym* t) { val = t; }
	TypeSym* getType() { return val; } 
	bool blockDefined() const { return body != 0; }
	bool canConvertTo(TypeSym* to);
	bool operator == (TypeSym* o) const;
	void generate(AsmCode& code) const;
};

class StructSym : public TypeSym
{
private:
	SymTable* fields;
public:
	friend class Parser;
	friend class SymTable;
	StructSym(const string& name, SymTable* f): TypeSym(name), fields(f) {}
	void print(int deep) const;
	bool isStruct() { return true; }
	string typeName() const;
};

extern ScalarSym* intType;
extern ScalarSym* floatType;
extern ScalarSym* charType;
extern ScalarSym* voidType;
extern map<TypeSym*, int> typePriority;
extern map<OperationsT, TypeSym*> operationTypeOperands;
extern map<OperationsT, TypeSym*> operationReturningType;

#endif
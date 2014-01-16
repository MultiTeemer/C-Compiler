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
	int offset;
	string name;
	Symbol(const string& n): name(n), offset(0) {}	
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
	int byteSize() const { return name == "char" ? 1 : name == "void" ? 0 : 4; }
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

class ArraySym : public TypeSym
{
private:
	int size;
	TypeSym* type;
public:
	friend class Parser;
	friend class VarSym;
	friend class BinaryOpNode;
	ArraySym(TypeSym* t, int s): TypeSym(""), type(t), size(s) {}
	string typeName() const;
	TypeSym* nextType() const { return type; }
	void setNextType(TypeSym* t) { type = t; }
	bool isLvalue() const { return true; }
	bool operator == (TypeSym* t) const;
	bool canConvertTo(TypeSym* t);
	void generate(AsmCode& code) const;
	int byteSize() const { return type->byteSize() * size; }
	PointerSym* convertToPointer() const { return new PointerSym(type); }
};

class VarSym : public Symbol
{
public:	
	bool global;
	TypeSym* type;
	VarSym(const string& n, TypeSym* t): Symbol(n), type(t), global(true) {} 
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
protected:
	vector<string> names;
	vector<Symbol*> symbols;
	map<string, int> index;
public:		
	int offset;
	int shift;
	friend class FuncSym;
	friend class FuncCallNode;
	SymTable(int tableShift = 0): names(0), symbols(0), offset(0), shift(tableShift) {}
	Symbol* find(const string& name) const;
	virtual void add(Symbol* s);
	void print(int deep = 0) const;
	void generateGlobals(AsmCode& code) const;
	void generateCode(AsmCode& code) const;
	void calculateSize();
	bool exists(const string& name);
	bool operator == (SymTable* o) const;
	bool operator != (SymTable* o) const { return !(*this == o); }
	int size() const;
	int byteSize() const;
};

class SymTableForLocals : public SymTable
{
public:
	SymTableForLocals(int off = 0): SymTable(off) {}
	void add(Symbol* s);
};

class SymTableForParams : public SymTable
{
public:
	SymTableForParams(): SymTable() { offset = 4; }
	void add(Symbol* s);
};

class SymTableForFields : public SymTable
{
public:
	SymTableForFields(): SymTable() {}
	void add(Symbol* s);
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
	SymTableForLocals* locals;
	vector<Statement*> statements;
public:
	friend class Parser;
	friend class FuncSym;
	Block(SymTableForLocals* l): locals(l), statements(0) {}
	void AddStatement(Statement* stmt) { statements.push_back(stmt); }
	void print(int deep) const; 
	void generate(AsmCode& code) const;
	int size() const { return statements.size(); }
};

class FuncSym : public TypeSym
{
private:
	SymTable* params;
	Block* body;
	TypeSym* val;
	AsmArgLabel* endLabel;
public:
	friend class Parser;
	friend class FuncCallNode;
	friend class ReturnStatement;
	FuncSym(TypeSym* v): TypeSym(""), val(v), params(0), body(0), endLabel(0) {} 
	string typeName() const;
	void print(int deep) const;	
	void setNextType(TypeSym* t) { val = t; }	
	void generate(AsmCode& code, const string& name) const;
	TypeSym* nextType() const { return val; }
	TypeSym* getType() { return val; } 
	bool blockDefined() const { return body != 0; }
	bool canConvertTo(TypeSym* to);
	bool operator == (TypeSym* o) const;

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
	bool canConvertTo(TypeSym* to);
	int byteSize() const { return fields ? fields->byteSize() : 0; }
	string typeName() const;
};

class AliasSym : public TypeSym
{
public:
	TypeSym* type;
	AliasSym(const string& name, TypeSym* t): TypeSym(name), type(t) {}
	void print(int deep) const;
	string typeName() const;
	TypeSym* getType() { return type; }
	int byteSize() const { return type->byteSize(); }
};

extern ScalarSym* intType;
extern ScalarSym* floatType;
extern ScalarSym* charType;
extern ScalarSym* voidType;
extern PointerSym* stringType;
extern map<TypeSym*, int> typePriority;
extern map<OperationsT, TypeSym*> operationTypeOperands;
extern map<OperationsT, TypeSym*> operationReturningType;

#endif
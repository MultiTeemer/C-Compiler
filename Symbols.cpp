#include <iostream>
#include "Symbols.h"

static const string lineSeparator("+------------------------------------------------------------------+\n");
static const int M = 2;

void Symbol::print(int deep) const
{
	cout << string(M * deep, ' ') << name << endl;
}

void VarSym::print(int deep) const
{
	cout << string(M * deep, ' ') << name;
	if (type->typeName().length() > 0)
		cout << ' ' << type->typeName() << endl;
	else 
		type->print(deep + 1);	
}

string TypeSym::typeName() const
{
	return name;
}

bool TypeSym::operator==(const string& o) const
{
	return name == o;
}

string ConstTypeSym::typeName() const
{
	return "const " + type->typeName();
}

string ArraySym::typeName() const
{
	//return "[" + (size != -1 ? to_string(size): "") + "]" + type->typeName();
	return "array[" + (size != -1 ? to_string(size): "") + "] of " + type->typeName();
}

string PointerSym::typeName() const
{
	return "pointer to " + type->typeName();
}

void AliasSym::print(int deep) const
{
	cout << string(M * deep, ' ') << name << " alias for " << type->typeName() << endl;
}

string AliasSym::typeName() const
{
	return type->typeName();
}

void StructSym::print(int deep) const
{
	cout << string(M * deep, ' ') << "struct " << name << endl;
	if (fields)
		fields->print(deep + 1);
}

string StructSym::typeName() const
{
	return "struct " + name;
}

void FuncSym::print(int deep) const
{
	cout << endl << string(M * deep, ' ') << "Function " << name << "()" << endl;
	if (params->size() > 0)
	{
		cout << string(M * deep, ' ') << "Params:" << endl;
		params->print(deep + 1);
	}
	if (body)
	{
		cout << endl;
		body->print(deep + 1);
		cout << endl;
	}
	cout << string(M * deep, ' ') << "Value:" << endl << string(M * 2 * deep, ' ') << val->typeName() << endl << endl;
}

string FuncSym::typeName() const
{
	string str = "function(";
	for (int i = 0; i < params->size(); i++)
	{
		str += dynamic_cast<VarSym*>(params->symbols[i])->type->typeName();
		if (i < params->size() - 1)
			str += ", "; 
	}
	str += ") returning " + val->typeName();
	return str;
}


Symbol* SymTable::find(const string& name) const
{
	return index.count(name) == 1 ? symbols[index.at(name)] : 0;
}

void SymTable::add(Symbol* symbol)
{
	symbols.push_back(symbol);
	names.push_back(symbol->name);
	index[symbol->name] = symbols.size() - 1;
}

void SymTable::print(int deep) const
{
	for (int i = 0; i < symbols.size(); i++)
	{
		cout << string(M * deep, ' ');
		symbols[i]->print(deep + 1);		
	}
}

int SymTable::size() const
{
	return symbols.size();
}

bool SymTable::exists(const string& name) 
{
	return find(name) != 0;
}

void SymTableStack::push(SymTable* table)
{
	tables.push_back(table);
}

void SymTableStack::pop()
{
	tables.pop_back();
}

SymTable* SymTableStack::top()
{
	return tables.size() > 0 ? tables.back() : 0;
}

Symbol* SymTableStack::find(const string& name) const
{
	Symbol* sym = 0;
	for (int i = tables.size() - 1; i >= 0 && !sym; i--)
		sym = tables[i]->find(name);
	return sym;
}

void SymTableStack::add(Symbol* symbol)
{
	top()->add(symbol);
}

void SymTableStack::print(int deep) const
{
	for (int i = 0; i < tables.size(); i++)
	{
		tables[i]->print(deep + 1);
		cout << lineSeparator;
	}
}

bool SymTableStack::existsInLastNamespace(const string& name)  
{
	return top()->find(name) != 0;
}

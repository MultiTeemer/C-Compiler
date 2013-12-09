#include <iostream>
#include "Symbols.h"

static const int M = 2;

void Symbol::print(int deep) const
{
	cout << string(M * deep, ' ') << name << endl;
}

void VarSym::print(int deep) const
{
	cout << string(M * deep, ' ') << name;
	if (dynamic_cast<FuncSym*>(type) && dynamic_cast<FuncSym*>(type)->blockDefined())
		type->print(deep + 1);
	else if (type->typeName().length() > 0)
		cout << ' ' << type->typeName() << endl;
	else 
		type->print(deep + 1);	
}

void VarSym::generate(AsmCode& code) const
{
	int size = type->byteSize();
	int dwords = size / 4 + (size % 4 != 0);
	code.add(cmdDD, makeArgMemory("var_" + name), makeArgDup(dwords));
}

string TypeSym::typeName() const
{
	return name;
}

bool TypeSym::operator==(const string& o) const
{
	return name == o;
}

bool ScalarSym::canConvertTo(TypeSym* to) 
{
	if (dynamic_cast<PointerSym*>(to) || dynamic_cast<FuncSym*>(to))
		return false;
	return typePriority[this] <= typePriority[to]; // how to do it with const-modifier?
}

string ConstTypeSym::typeName() const
{
	return "const " + type->typeName();
}

string ArraySym::typeName() const
{
	return "array[" + (size != -1 ? to_string(size) : "") + "] of " + type->typeName();
}

void ArraySym::generate(AsmCode& code) const
{

}

bool ArraySym::operator==(TypeSym* o) const
{
	ArraySym* arr = dynamic_cast<ArraySym*>(o);
	if (!arr)
		return false;
	return size == arr->size && *type == arr->type;
}

bool ArraySym::canConvertTo(TypeSym* to)
{
	if (to == intType)
		return true;
	PointerSym* p = dynamic_cast<PointerSym*>(to);
	if (p && *p->type == type)
		return true;
	return false;
}

string PointerSym::typeName() const
{
	return "pointer to " + type->typeName();
}

bool PointerSym::operator==(TypeSym* o) const
{
	PointerSym* p = dynamic_cast<PointerSym*>(o);
	if (!p)
		return false;
	TypeSym* type1 = nextType();
	TypeSym* type2 = p->nextType();
	while (true)
	{
		if (*type1 != type2)
			return false;
		TypeSym* tmp1 = type1->nextType();
		TypeSym* tmp2 = type2->nextType();
		if (tmp1 == 0 || tmp2 == 0)
		{
			if (tmp1 != 0 || tmp2 != 0)
				return false;
			return true;
		} else {
			type1 = tmp1;
			type2 = tmp2;
		}
	}
}

bool PointerSym::canConvertTo(TypeSym* to)
{
	if (to == intType)
		return true;
	PointerSym* pointer = dynamic_cast<PointerSym*>(to);
	if (pointer)
		return *this == pointer;
	return false;
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

bool StructSym::canConvertTo(TypeSym* to) 
{
	StructSym* struc = dynamic_cast<StructSym*>(to);
	if (!struc || *fields != struc->fields)
		return false;
	return true;
}

string StructSym::typeName() const
{
	return "struct " + name;
}

bool FuncSym::canConvertTo(TypeSym* o) 
{
	return *this == o;
}

bool FuncSym::operator==(TypeSym* o) const
{
	FuncSym* func = dynamic_cast<FuncSym*>(o);
	if (!func)
		return false;
	return *params == func->params && *val == func->val;
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

void FuncSym::generate(AsmCode& code, const string& name) const
{
	code.add(makeLabel("f_" + name))
		.add(cmdPUSH, EBP)
		.add(cmdMOV, EBP, ESP);
	body->generate(code);
	code.add(cmdMOV, ESP, EBP)
		.add(cmdPOP, EBP)
		.add(cmdRET, makeArg(params->byteSize()));
}

Symbol* SymTable::find(const string& name) const
{
	if (index.count(name))
		return symbols[index.at(name)];
	else if (index.count('$' + name))
		return symbols[index.at('$' + name)];
	return 0;
}

void SymTable::add(Symbol* symbol)
{
	symbols.push_back(symbol);
	names.push_back(symbol->name);
	index[symbol->name] = symbols.size() - 1;
	symbol->offset = offset;
	offset += symbol->byteSize();
}

void SymTableForLocals::add(Symbol* symbol)
{
	SymTable::add(symbol);
	offset -= 2 * symbol->byteSize();
	if (dynamic_cast<VarSym*>(symbol))
		dynamic_cast<VarSym*>(symbol)->global = false;
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

int SymTable::byteSize() const
{
	int bytes = 0;
	for (int i = 0; i < size(); i++)
		bytes += symbols[i]->byteSize();
	return bytes;
}

bool SymTable::exists(const string& name) 
{
	return find(name) != 0;
}

bool SymTable::operator==(SymTable* o) const
{
	if (size() != o->size())
		return false;
	for (int i = 0; i < size(); i++)
		if (*dynamic_cast<VarSym*>(symbols[i])->type != dynamic_cast<VarSym*>(o->symbols[i])->type)
			return false;
	return true;
}

void SymTable::generateGlobals(AsmCode& code) const
{
	for (int i = 0; i < size(); i++)
	{
		VarSym* sym = dynamic_cast<VarSym*>(symbols[i]);
		if (sym && !dynamic_cast<FuncSym*>(sym->type))
			sym->generate(code);
	}
}

void SymTable::generateCode(AsmCode& code) const
{
	for (int i = 0; i < size(); i++)
	{
		VarSym* sym = dynamic_cast<VarSym*>(symbols[i]);
		if (sym && dynamic_cast<FuncSym*>(sym->type))
			dynamic_cast<FuncSym*>(sym->type)->generate(code, sym->name);
	}
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

void SingleStatement::generate(AsmCode& code) const
{
	expr->generate(code);
	if (expr->getType())
		code.add(cmdPOP, makeArg(EAX));
}

void Block::print(int deep) const
{
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

void Block::generate(AsmCode& code) const
{
	code.add(cmdSUB, ESP, locals->byteSize());
	for (int i = 0; i < size(); i++)
		statements[i]->generate(code);
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

void WhilePreCondStatement::generate(AsmCode& code) const
{

}

void WhilePostCondStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "do .. while ()" << endl;
	CycleStatement::print(deep);
}

void WhilePostCondStatement::generate(AsmCode& code) const
{

}

void ForStatement::print(int deep) const
{
	string tab(deep * M, ' ');
	cout << tab << "for (;;) .." << endl;
	cout << tab << "First action:" << endl;
	initialization->print(deep + 1);
	cout << tab << "Condition:" << endl;
	condition->print(deep + 1);
	cout << tab << "Step:" << endl;
	increment->print(deep + 1);
	cout << tab << "Block:" << endl;
	body->print(deep + 1);
}

void ForStatement::generate(AsmCode& code) const
{

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

void IfStatement::generate(AsmCode& code) const
{

}

void ContinueStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "continue" << endl;
}

void ContinueStatement::generate(AsmCode& code) const
{

}

void BreakStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "break" << endl;
}

void BreakStatement::generate(AsmCode& code) const
{

}

void ReturnStatement::print(int deep) const
{
	cout << string(deep * M, ' ') << "return" << endl;
	if (arg)
		arg->print(deep + 1);
}

void ReturnStatement::generate(AsmCode& code) const
{

}

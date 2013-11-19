#include <iostream>
#include "Nodes.h"
#include "Exceptions.h"

using namespace std;

ScalarSym* intType = new ScalarSym("int");
ScalarSym* floatType = new ScalarSym("float");
ScalarSym* charType = new ScalarSym("char");
ScalarSym* voidType = new ScalarSym("void");

map<TypeSym*, int> typePriority;
map<OperationsT, TypeSym*> operationTypeOperands;
map<OperationsT, TypeSym*> operationReturningType;

static const int M = 2;

Node* Node::makeTypeCoerce(Node* expr, TypeSym* from, TypeSym* to)
{
	if (!from->canConvertTo(to))
		throw CompilerException("Cannot perform conversion", expr->token->line, expr->token->col);
	if (from == to || *from == to)
		return expr;
	if (!dynamic_cast<ScalarSym*>(from) || !dynamic_cast<ScalarSym*>(to))
	{
		return expr;
	} else {
		if (typePriority[to] - typePriority[from] == 1)
			return new CoerceNode(0, expr, to);
		return new CoerceNode(0, makeTypeCoerce(expr, from, intType), floatType); 
	}
}

void EmptyNode::print(int deep) const
{ 
	cout << string(deep * M, ' ') << "<empty expression>" << endl;
}

string OpNode::opName() const
{
	char operations[][5] = { "+", "-", "*", "/", "%", "++", "--", "+=", "-=", "*=", "/=", "%=", "?", ":",
		"&", "|", "&&", "||", "!", "^", "&=", "|=", "^=", "=", "==", "!=", ">", "<", ">=", "<=", "<<", ">>", ".", "->",
		"<<=", ">>=", "(", ")", "[", "]", ",", "~" };
	return string(operations[dynamic_cast<OpToken*>(token)->val]);
}

BinaryOpNode::BinaryOpNode(Token* op, Node* l, Node* r): OpNode(op), left(l), right(r) 
{
	if (l == 0 || r == 0)
		throw ParserException("Lost operand", op->line, op->col);
}

TypeSym* BinaryOpNode::getType() const
{
	TypeSym* leftType = left->getType();
	TypeSym* rightType = right->getType();
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	TypeSym* maxTypeOfArgs = 0;
	if (operationTypeOperands.count(op))
		maxTypeOfArgs = operationTypeOperands[op];
	else
		maxTypeOfArgs = typePriority[leftType] > typePriority[rightType] ? leftType : rightType;
	PointerSym* lp = dynamic_cast<PointerSym*>(leftType);
	PointerSym* rp = dynamic_cast<PointerSym*>(rightType);
	switch (op)
	{
	case ASSIGN:
	case MULT_ASSING:
	case PLUS_ASSING:
	case MINUS_ASSING:
	case DIV_ASSING:
		if (!leftType->isLvalue())
			throw CompilerException("Left argument of assignment must be lvalue", left->token->line, left->token->col);
		right = makeTypeCoerce(right, rightType, leftType);
		return leftType;
	case DOT:
		if (!dynamic_cast<StructSym*>(leftType))
			throw CompilerException("Left operand of . must be a structure", left->token->line, left->token->col);
		return rightType;
	case ARROW:
		if (!lp || !dynamic_cast<StructSym*>(lp->type))
			throw CompilerException("Left operand of -> must be of pointer-to-structure type", left->token->line, left->token->col);
		return rightType;
	case MINUS:		
		if (lp && rp)
			return intType;
	case PLUS:
		if (lp && rp)
			throw CompilerException("Cannot add two pointers", token->line, token->col);
		if (lp || rp)
			return lp == 0 ? rightType : leftType;		
	default:
		if (typePriority[maxTypeOfArgs] < max(typePriority[leftType], typePriority[rightType]))
			throw CompilerException("Invalid type of operands", token->line, token->col);
		left = makeTypeCoerce(left, leftType, maxTypeOfArgs);
		right = makeTypeCoerce(right, rightType, maxTypeOfArgs);
		if (operationReturningType.count(op))
			return operationReturningType[op];
		else 
			return maxTypeOfArgs;
	}	
}

void BinaryOpNode::print(int deep) const
{
	left->print(deep + 1);
	cout << string(deep * M, ' ') << opName() << endl;
	right->print(deep + 1);
}

void IntNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<IntegerToken*>(token)->val << endl;
}

TypeSym* IntNode::getType() const
{
	return intType;
}

void FloatNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<FloatToken*>(token)->val << endl;
}

TypeSym* FloatNode::getType() const
{
	return floatType;
}

void IdentifierNode::print(int deep) const
{
	cout << string(deep * M, ' ') << sym->name << endl;
}

TypeSym* IdentifierNode::getType() const
{
	return sym->getType();
}

UnaryOpNode::UnaryOpNode(Token* op, Node* oper): OpNode(op), operand(oper) 
{
	if (!oper)
		throw ScannerException("Lost operand", op->line, op->col);
}

void UnaryOpNode::print(int deep) const
{
	cout << string(deep * M, ' ') << opName() << endl;
	operand->print(deep + 1);
}

TypeSym* UnaryOpNode::getType() const
{
	TypeSym* type = operand->getType();
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	TypeSym* maxType = operationTypeOperands.count(op) ? intType : floatType;
	if (typePriority[maxType] < typePriority[type])
		throw CompilerException("Invalid type of unary operation", token->line, token->col);
	if (op == MULT)
	{
		if (!dynamic_cast<PointerSym*>(type))
			throw CompilerException("Type of unary operation is not a pointer", token->line, token->col);
		return dynamic_cast<PointerSym*>(type)->type;
	}
	return type;	
}

void PostfixUnaryOpNode::print(int deep) const
{
	operand->print(deep);
	cout << string(deep * M, ' ') << opName() << endl;
}

void CoerceNode::print(int deep) const
{
	cout << string(deep * M, ' ') << type->name;
	cout << string(deep * M, ' ') << "(" << endl;
	operand->print(deep + 1);
	cout << string(deep * M, ' ') << ")" << endl;
}

TypeSym* CoerceNode::getType() const
{
	return type;
}

void FunctionalNode::printArgs(int deep) const
{
	for (int i = 0; i < args.size(); i++)
	{
		args[i]->print(deep + 1);
		if (i < args.size() - 1)
			cout << string(deep * M, ' ') << ',' << endl;
	}
}

void FuncCallNode::print(int deep) const
{
	name->print(deep);
	cout << string(deep * M, ' ') << "(" << endl;
	printArgs(deep);
	cout << string(deep * M, ' ') << ")" << endl;
}

TypeSym* FuncCallNode::getType() const
{
	FuncSym* sym = dynamic_cast<FuncSym*>(symbol);
	int formalParametersCount = sym->params->size();
	int realParametersCount = args.size();
	if (formalParametersCount != realParametersCount)
		throw CompilerException("Incorrect parameters count", token->line, token->col);
	for (int i = 0; i < formalParametersCount; i++)
	{
		TypeSym* realParamType = args[i]->getType();
		TypeSym* formalParamType = sym->params->symbols[i]->getType();
		if (!realParamType->canConvertTo(formalParamType))
			throw CompilerException("Invalid type of parameter", args[i]->token->line, args[i]->token->col);
		args[i] = makeTypeCoerce(args[i], realParamType, formalParamType);
	}
	return symbol->getType();
}

void ArrNode::print(int deep) const 
{
	name->print(deep);
	cout << string(deep * M, ' ') << "[" << endl;
	printArgs(deep);
	cout << string(deep * M, ' ') << "]" << endl;
}

TypeSym* ArrNode::getType() const
{
	ArraySym* sym = dynamic_cast<ArraySym*>(name->getType());
	if (!sym)
		throw CompilerException("Expression must have a pointer-to-object type", name->token->line, name->token->col);
	TypeSym* type = sym->nextType();
	TypeSym* res = 0;
	for (int i = 0; i < args.size(); i++)
	{
		if (type == 0)
			throw CompilerException("Expression must have a pointer-to-object type", args[i]->token->line, args[i]->token->col);
		if (!args[i]->getType()->canConvertTo(intType))
			throw CompilerException("Expression must have integral type", args[i]->token->line, args[i]->token->col);
		args[i] = makeTypeCoerce(args[i], args[i]->getType(), intType);		
		if (i == args.size() - 1)
			res = type;
		// !!!!
		type = type->nextType();
	}
	return res;
}

string KeywordNode::KeywordName() const
{
	switch (dynamic_cast<KeywordToken*>(token)->val)
	{
	case CHAR:
		return string("char");
	case INT:
		return string("int");
	case FLOAT:
		return string("float");
	case SIZEOF:
		return string("sizeof");
	default:
		throw ParserException("unknown function", token->line, token->col);
	}
}

void KeywordNode::print(int deep) const
{
	cout << string(deep * M, ' ') << KeywordName() << endl;
}

void CharNode::print(int deep) const
{
	cout << string(deep * M, ' ') << '\'' << dynamic_cast<CharToken*>(token)->val << '\'' << endl; 
}

TypeSym* CharNode::getType() const
{
	return charType;
}

void StringNode::print(int deep) const 
{ 
	cout << string(deep * M, ' ') << '"' << dynamic_cast<StringToken*>(token)->val << '"' << endl; 
}

TypeSym* StringNode::getType() const
{
	return new PointerSym(charType);
}

void TernaryOpNode::print(int deep) const
{
	string tab = string(deep * M, ' ');
	cout << tab << "(" << endl;
	condition->print(deep + 1);
	cout << tab << ")" << endl;
	cout << tab << "?" << endl;
	left->print(deep + 1);
	cout << tab << ":" << endl;
	right->print(deep + 1);
}

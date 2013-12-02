#include <iostream>
#include "Nodes.h"
#include "Exceptions.h"

using namespace std;

ScalarSym* intType = new ScalarSym("int");
ScalarSym* floatType = new ScalarSym("float");
ScalarSym* charType = new ScalarSym("char");
ScalarSym* voidType = new ScalarSym("void");
PointerSym* stringType = new PointerSym(charType);

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
	case MOD_ASSIGN:
	case AND_ASSIGN:
	case OR_ASSIGN:
	case XOR_ASSIGN:
	case BITWISE_SHIFT_LEFT_ASSIGN:
	case BITWISE_SHIFT_RIGHT_ASSIGN:	
		if (!leftType->canConvertTo(intType) || !rightType->canConvertTo(intType))
			throw CompilerException("Invalid operator arguments type (required int in both sides)", token->line, token->col);
		// fallthrough
	case ASSIGN:
	case MULT_ASSIGN:
	case PLUS_ASSIGN:
	case MINUS_ASSIGN:
	case DIV_ASSIGN:
		if (!left->isModifiableLvalue())
			throw CompilerException("Left argument of assignment must be modifiable lvalue", left->token->line, left->token->col);
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

bool BinaryOpNode::isModifiableLvalue() const
{
	switch (dynamic_cast<OpToken*>(token)->val)
	{
	case ASSIGN:
	case PLUS_ASSIGN:
	case MINUS_ASSIGN:
	case MULT_ASSIGN:
	case DIV_ASSIGN:
	case MOD_ASSIGN:
	case BITWISE_SHIFT_LEFT_ASSIGN:
	case BITWISE_SHIFT_RIGHT_ASSIGN:
	case XOR_ASSIGN:
	case AND_ASSIGN:
	case OR_ASSIGN:
		return left->isModifiableLvalue();
	case DOT:
		return right->isModifiableLvalue();
	default:
		return false;
	}
}

bool BinaryOpNode::isLvalue() const
{
	switch (dynamic_cast<OpToken*>(token)->val)
	{
	case DOT:
	case ARROW:
		return right->isLvalue();
	default:
		return isModifiableLvalue();
	}
}

void BinaryOpNode::print(int deep) const
{
	left->print(deep + 1);
	cout << string(deep * M, ' ') << opName() << endl;
	right->print(deep + 1);
}

bool BinaryOpNode::isAssignment(OperationsT op) 
{
	return op == ASSIGN || op == PLUS_ASSIGN || op == MINUS_ASSIGN 
		|| op == MULT_ASSIGN || op == DIV_ASSIGN || op == MOD_ASSIGN 
		|| op == AND_ASSIGN || op == XOR_ASSIGN || op == OR_ASSIGN
		|| op == BITWISE_SHIFT_LEFT_ASSIGN || op == BITWISE_SHIFT_RIGHT_ASSIGN;
}

void BinaryOpNode::generate(AsmCode& code) const
{
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	right->generate(code);
	if (op == ASSIGN)
	{
		code.add(cmdPOP, EAX);
		if (!dynamic_cast<IdentifierNode*>(left))
			throw CompilerException("Cannot perform assign into non-variable", left->token->line, left->token->col);
		code.add(cmdMOV, makeArgMemory("var_" + left->token->text), makeArg(EAX));
	} else {
		AsmArg *l, *r;
		if (isAssignment(op))
		{
			l = makeArgMemory("var_" + left->token->text);
			r = makeArg(EBX);
			code.add(cmdPOP, EBX);
		} else {
			l = makeArg(EAX);
			r = makeArg(EBX);
			left->generate(code);	
			code.add(cmdPOP, EAX)
				.add(cmdPOP, EBX);
		} 
		if (op == COMMA) 
			code.add(cmdMOV, EAX, EBX);			
		else if (op == DIV || op == DIV_ASSIGN || op == MOD || op == MOD_ASSIGN) {
			code.add(cmdCDQ);
			if (!isAssignment(op))
			{
				code.add(cmdIDIV, EBX);
				if (op == MOD)
					code.add(cmdMOV, EAX, EDX);
			} else
				code.add(cmdMOV, makeArg(EAX), l)
					.add(cmdIDIV, r)
					.add(cmdMOV, l, makeArg(op == MOD_ASSIGN ? EDX : EAX));
		} else if (op == MULT_ASSIGN)
			code.add(cmdIMUL, r, l)
				.add(cmdMOV, l, r);
		else if (op == BITWISE_SHIFT_LEFT || op == BITWISE_SHIFT_RIGHT) 
			code.add(cmdMOV, makeArg(ECX), r)
				.add(op == BITWISE_SHIFT_LEFT ? cmdSHL : cmdSHR, l, makeArg(CL));
		else {
			AsmCommandsT cmd;
			switch (op)
			{
			case PLUS:
			case PLUS_ASSIGN:
				cmd = cmdADD;
				break;
			case MINUS:
			case MINUS_ASSIGN:
				cmd = cmdSUB;
				break;
			case MULT:
				cmd = cmdIMUL;
				break;
			case BITWISE_AND:
			case AND_ASSIGN:
				cmd = cmdAND;
				break;
			case BITWISE_OR:
			case OR_ASSIGN:
				cmd = cmdOR;
				break;
			case BITWISE_XOR:
			case XOR_ASSIGN:
				cmd = cmdXOR;
				break;
			} 			
			code.add(cmd, l, r);
		}
	}
	code.add(cmdPUSH, EAX);
}

void IntNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<IntegerToken*>(token)->val << endl;
}

void IntNode::generate(AsmCode& code) const
{
	code.add(cmdPUSH, makeArg(dynamic_cast<IntegerToken*>(token)->val));
}

TypeSym* IntNode::getType() const
{
	return intType;
}

void FloatNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<FloatToken*>(token)->val << endl;
}

void FloatNode::generate(AsmCode& code) const
{

}

TypeSym* FloatNode::getType() const
{
	return floatType;
}

void IdentifierNode::print(int deep) const
{
	cout << string(deep * M, ' ') << sym->name << endl;
}

void IdentifierNode::generate(AsmCode& code) const
{
	if (sym->global)
		code.add(cmdPUSH, makeArgMemory("dword ptr [var_" + dynamic_cast<IdentifierToken*>(token)->val+ "]"));
	else
		code.add(cmdPUSH, makeIndirectArg(EBP, sym->offset));
}

TypeSym* IdentifierNode::getType() const
{
	return sym->getType();
}

bool IdentifierNode::isModifiableLvalue() const 
{
	TypeSym* type = dynamic_cast<VarSym*>(sym)->type;
	return !dynamic_cast<ConstTypeSym*>(type) && !dynamic_cast<FuncSym*>(type) 
		&& !dynamic_cast<StructSym*>(type) && !dynamic_cast<ArraySym*>(type);
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
	switch (op)
	{
	case MULT:
		if (!dynamic_cast<PointerSym*>(type))
			throw CompilerException("Type of unary operation is not a pointer", token->line, token->col);
		return dynamic_cast<PointerSym*>(type)->type;
	case BITWISE_AND:
		if (!operand->isLvalue())
			throw CompilerException("Expression must have lvalue", token->line, token->col);
		return new PointerSym(type);
		break;
	case BITWISE_NOT:
		operand = makeTypeCoerce(operand, type, intType);
		break;
	case LOGICAL_NOT:
		if (dynamic_cast<StructSym*>(type))
			throw CompilerException("Cannot perform logical not operation over structure", token->line, token->col);
		break;
	case DEC:
	case INC:
		if (!operand->isModifiableLvalue())
			throw CompilerException("Expression must have modifiable lvalue", token->line, token->col);
		break;
	case MINUS:
		if (!type->canConvertTo(floatType))
			throw CompilerException("Expression must have arithmetic type", token->line, token->col);
	}
	return type;	
}

bool UnaryOpNode::isModifiableLvalue() const
{
	OperationsT op = dynamic_cast<OpToken*>(token)->val; 
	return (op == MULT || op == DEC || op == INC) && getType()->isModifiableLvalue();
}

bool UnaryOpNode::isLvalue() const 
{
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	return op == MULT || ((op == DEC || op == INC) && operand->isLvalue());
}

void UnaryOpNode::generate(AsmCode& code) const
{
	operand->generate(code);
	code.add(cmdPOP, EAX);
	switch (dynamic_cast<OpToken*>(token)->val)
	{
	case MINUS:
		code.add(cmdNEG, EAX);
		break;
	case BITWISE_NOT:
		code.add(cmdNOT, EAX);
		break;
	default:
		break;
	}
	code.add(cmdPUSH, EAX);
}

void PostfixUnaryOpNode::print(int deep) const
{
	operand->print(deep);
	cout << string(deep * M, ' ') << opName() << endl;
}

void PostfixUnaryOpNode::generate(AsmCode& code) const
{

}

void CoerceNode::print(int deep) const
{
	cout << string(deep * M, ' ') << type->name;
	cout << string(deep * M, ' ') << "(" << endl;
	operand->print(deep + 1);
	cout << string(deep * M, ' ') << ")" << endl;
}

void CoerceNode::generate(AsmCode& code) const
{

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

void FuncCallNode::generate(AsmCode& code) const
{

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

void ArrNode::generate(AsmCode& code) const
{

}

TypeSym* ArrNode::getType() const
{
	ArraySym* sym = dynamic_cast<ArraySym*>(name->getType());
	if (!sym)
		throw CompilerException("Expression must have a pointer-to-object type", name->token->line, name->token->col);
	TypeSym* type = sym;
	for (int i = 0; i < args.size(); i++)
	{
		type = type->nextType();
		if (type == 0)
			throw CompilerException("Expression must have a pointer-to-object type", args[i]->token->line, args[i]->token->col);
		if (!args[i]->getType()->canConvertTo(intType))
			throw CompilerException("Expression must have integral type", args[i]->token->line, args[i]->token->col);
		args[i] = makeTypeCoerce(args[i], args[i]->getType(), intType);			
	}
	return type;
}

bool ArrNode::isModifiableLvalue() const
{
	TypeSym* type = name->getType();
	for (int i = 0; i < args.size(); i++)
		type = type->nextType();
	return type->isModifiableLvalue();
}

void IOOperatorNode::generate(AsmCode& code) const
{
	for (int i = args.size() - 1; i > -1; i--)
		args[i]->generate(code);
	code.add(token->val, makeArgMemory("str" + to_string(format->index)));
	for (int i = 0; i < args.size(); i++)
		code.add(cmdPOP, EAX);
}

void IOOperatorNode::print(int deep) const
{
	string tab(deep * M, ' ');
	cout << tab << token->text << "(" << endl;
	format->print(deep + 1);
	if (args.size() > 0)
	{
		cout << tab << ',' << endl;
		printArgs(deep + 1);
	}	
	cout << tab << ")" << endl;
}

string KeywordNode::KeywordName() const
{
	return token->text;
}

void KeywordNode::print(int deep) const
{
	cout << string(deep * M, ' ') << KeywordName() << endl;
}

void CharNode::print(int deep) const
{
	cout << string(deep * M, ' ') << '\'' << dynamic_cast<CharToken*>(token)->val << '\'' << endl; 
}

void CharNode::generate(AsmCode& code) const
{

}

TypeSym* CharNode::getType() const
{
	return charType;
}

void StringNode::print(int deep) const 
{ 
	cout << string(deep * M, ' ') << '"' << dynamic_cast<StringToken*>(token)->val << '"' << endl; 
}

void StringNode::generate(AsmCode& code) const
{
	code.add(cmdDB, makeArgMemory("str" + to_string(index)), makeString(token->text));
}

TypeSym* StringNode::getType() const
{
	return stringType;
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

#include <iostream>
#include "Nodes.h"
#include "Exceptions.h"

using namespace std;

ScalarSym* intType = new ScalarSym("int");
ScalarSym* floatType = new ScalarSym("float");
ScalarSym* charType = new ScalarSym("char");
ScalarSym* voidType = new ScalarSym("void");
PointerSym* stringType = new PointerSym(charType);

string real4name("tmp4");
string real8name("tmp8");
AsmArgMemory* real4 = new AsmArgMemory(real4name, false);
AsmArgMemory* real8 = new AsmArgMemory(real8name, false);

map<TypeSym*, int> typePriority;
map<OperationsT, TypeSym*> operationTypeOperands;
map<OperationsT, TypeSym*> operationReturningType;

static const int M = 2;

void Node::generateByteToFPU(AsmCode& code) const
{
	code.add(cmdPOP, EAX)
		.add(cmdMOV, real4, makeArg(EAX))
		.add(cmdFLD, real4);
}

void Node::generateST0ToStack(AsmCode& code) const
{
	code.add(cmdFSTP, real4)
		.add(cmdPUSH, real4);
}

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
	if (dynamic_cast<AliasSym*>(leftType))
		leftType = leftType->getType();
	if (dynamic_cast<AliasSym*>(rightType))
		rightType = rightType->getType();
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	TypeSym* maxTypeOfArgs = 0;
	if (operationTypeOperands.count(op))
		maxTypeOfArgs = operationTypeOperands[op];
	else
		maxTypeOfArgs = typePriority[leftType] > typePriority[rightType] ? leftType : rightType;
	PointerSym* lp = dynamic_cast<PointerSym*>(leftType);
	PointerSym* rp = dynamic_cast<PointerSym*>(rightType);
	ArraySym* la = dynamic_cast<ArraySym*>(leftType);
	ArraySym* ra = dynamic_cast<ArraySym*>(rightType);
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
		if (leftType->isStruct() && *leftType == rightType)
			return leftType;
		// fallthrough
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
		if (!lp || (!dynamic_cast<StructSym*>(lp->type) && !dynamic_cast<StructSym*>(dynamic_cast<AliasSym*>(lp->type)->type)))
			throw CompilerException("Left operand of -> must be of pointer-to-structure type", left->token->line, left->token->col);
		return rightType;		
	case MINUS:		
		if (lp && rp || la && ra)
		{
			if (lp && rp && *lp->type != rp->type 
				|| la && ra && *la->type != ra->type)
				throw CompilerException("Operand types are incompatible", token->line, token->col);
			return intType;
		}			
	case PLUS:
		if (lp && rp || la && ra)
			throw CompilerException("Cannot add two pointers", token->line, token->col);
		if (lp || rp)
			return lp == 0 ? rightType : leftType;
		if (la || ra)
			return new PointerSym(la == 0 ? ra->type : la->type);
	default:
		if (leftType->isStruct() || rightType->isStruct())
			throw CompilerException("Cannot perform operation over two structures", token->line, token->col);
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
	case ARROW:
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

bool BinaryOpNode::isComparison(OperationsT op)
{
	return op == EQUAL || op == LESS || op == GREATER
		|| op == LESS_OR_EQUAL || op == GREATER_OR_EQUAL 
		|| op == NOT_EQUAL;
}

void BinaryOpNode::generateForFloat(AsmCode& code) const
{
	left->generateLoadInFPUStack(code);
	right->generateLoadInFPUStack(code);
	AsmCommandsT cmd;
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	 if (isComparison(op)) 
	 {
		code.add(cmdFCOMPP)
			.add(cmdFNSTSW, AX)
			.add(cmdSAHF)
			.add(cmdMOV, EAX, 0);
		switch (op)
		{
		case GREATER:
			cmd = cmdSETG;
			break;
		case GREATER_OR_EQUAL:
			cmd = cmdSETGE;
			break;
		case LESS:
			cmd = cmdSETL;
			break;
		case LESS_OR_EQUAL:
			cmd = cmdSETLE;
			break;
		case EQUAL:
			cmd = cmdSETE;
			break;
		case NOT_EQUAL:
			cmd = cmdSETNE;
			break;
		}
		code.add(cmd, AL)
			.add(cmdPUSH, EAX);
	} else {
		switch (op)
		{
		case PLUS:
		case PLUS_ASSIGN:
			cmd = cmdFADDP;
			break;
		case MINUS:
		case MINUS_ASSIGN:
			cmd = cmdFSUBP;
			break;
		case DIV:
		case DIV_ASSIGN:
			cmd = cmdFDIVP;
			break;
		case MULT:
		case MULT_ASSIGN:
			cmd = cmdFMULP;
			break;
		default:
			throw CompilerException("not implemented", token->line, token->col);
			break;
		}
		code.add(cmd);
		generateST0ToStack(code);
		if (isAssignment(op))
		{
			left->generateLvalue(code);
			code.add(cmdPOP, EAX)
				.add(cmdPOP, EBX)
				.add(cmdMOV, makeIndirectArg(EAX), makeArg(EBX))
				.add(cmdPUSH, EBX);
		}
	}
}

void BinaryOpNode::generateLoadInFPUStack(AsmCode& code) const
{
	generate(code);
	generateByteToFPU(code);
}

void BinaryOpNode::generate(AsmCode& code) const
{
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	TypeSym* leftType = left->getType();
	TypeSym* rightType = right->getType();
	PointerSym* lp = dynamic_cast<PointerSym*>(leftType);
	PointerSym* rp = dynamic_cast<PointerSym*>(rightType);
	if (!lp && dynamic_cast<ArraySym*>(leftType))
		lp = dynamic_cast<ArraySym*>(leftType)->convertToPointer();
	if (!rp && dynamic_cast<ArraySym*>(rightType))
		rp = dynamic_cast<ArraySym*>(rightType)->convertToPointer();
	if ((op == PLUS || op == MINUS) && (lp || rp) && !(lp && rp))
	{
		if (!lp)
		{
			swap(left, right);
			swap(lp, rp);
		}
		left->generateLvalue(code);
		right->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdMOV, EBX, lp->type->byteSize())
			.add(cmdIMUL, EBX, EAX)
			.add(cmdPOP, EAX)
			.add(op == PLUS ? cmdADD : cmdSUB, EAX, EBX)
			.add(cmdPUSH, EAX);
		return;
	}
	if (op == MINUS && lp && rp)
	{
		left->generate(code);
		right->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdPOP, EBX)
			.add(cmdSUB, EAX, EBX)
			.add(cmdMOV, EBX, lp->type->byteSize())
			.add(cmdCDQ)
			.add(cmdIDIV, EBX)
			.add(cmdPUSH, EAX);
		return;
	}
	if (op == DOT || op == ARROW) {
			generateLvalue(code);
			code.add(cmdPOP, EAX)
				.add(cmdPUSH, makeIndirectArg(EAX));
	} else {
		right->generate(code);
		if (op == ASSIGN)
		{
			left->generateLvalue(code);
			code.add(cmdPOP, EAX);
			int size = right->getType()->byteSize();
			int steps = size / 4 + (size % 4 != 0);
			for (int i = 0; i < steps; i++)
				code.add(cmdPOP, EBX)
					.add(cmdMOV, makeIndirectArg(EAX, i * 4), makeArg(EBX));
			code.add(cmdMOV, EAX, EBX);
		} else {
			if (*getType() == floatType)
			{
				generateForFloat(code);
				return;
			}
			AsmArg *l, *r;
			if (isAssignment(op))
			{
				left->generateLvalue(code);
				l = makeIndirectArg(EAX);
				r = makeArg(EBX);				
				code.add(cmdPOP, EAX)
					.add(cmdPOP, EBX);
			} else {
				l = makeArg(EAX);
				r = makeArg(EBX);
				left->generate(code);	
				code.add(cmdPOP, EAX)
					.add(cmdPOP, EBX);
			} 
			if (op == COMMA) 
				code.add(cmdMOV, EAX, EBX);			
			else if (op == DIV || op == MOD) {
				code.add(cmdCDQ)
					.add(cmdIDIV, r);
				if (op == MOD)
					code.add(cmdMOV, l, makeArg(EDX));
			} else if (op == DIV_ASSIGN || op == MOD_ASSIGN) {
				code.add(cmdMOV, ECX, EAX)
					.add(cmdMOV, makeArg(EAX), makeIndirectArg(ECX))
					.add(cmdCDQ)
					.add(cmdIDIV, r)
					.add(cmdMOV, makeIndirectArg(ECX), makeArg(op == MOD_ASSIGN ? EDX : EAX));
				if (op == MOD_ASSIGN)
					code.add(cmdMOV, EAX, EDX);
			} else if (op == MULT_ASSIGN)
				code.add(cmdIMUL, r, l)
					.add(cmdMOV, l, r)
					.add(cmdMOV, makeArg(EAX), r);
			else if (op == BITWISE_SHIFT_LEFT || op == BITWISE_SHIFT_RIGHT 
				|| op == BITWISE_SHIFT_LEFT_ASSIGN || op == BITWISE_SHIFT_RIGHT_ASSIGN) {
				code.add(cmdMOV, makeArg(ECX), r)
					.add(op == BITWISE_SHIFT_LEFT || op == BITWISE_SHIFT_LEFT_ASSIGN ? cmdSHL : cmdSHR, l, makeArg(CL));
			} else if (isComparison(op)) {
				code.add(cmdCMP, l, r)
					.add(cmdMOV, EAX, 0);
				AsmCommandsT cmd;
				switch (op)
				{
				case LESS:
					cmd = cmdSETL;
					break;
				case LESS_OR_EQUAL:
					cmd = cmdSETLE;
					break;
				case GREATER:
					cmd = cmdSETG;
					break;
				case GREATER_OR_EQUAL:
					cmd = cmdSETGE;
					break;
				case EQUAL:
					cmd = cmdSETE;
					break;
				case NOT_EQUAL:
					cmd = cmdSETNE;;
				}
				code.add(cmd, AL);
			} else if (op == LOGICAL_AND || op == LOGICAL_OR) 
				code.add(cmdCMP, EAX, 0)
					.add(cmdMOV, EAX, 0)
					.add(cmdSETNE, AL) 
					.add(cmdCMP, EBX, 0)
					.add(cmdMOV, EBX, 0)
					.add(cmdSETNE, BL)
					.add(op == LOGICAL_AND ? cmdIMUL : cmdADD, EAX, EBX)
					.add(cmdCMP, EAX, 0)
					.add(cmdMOV, EAX, 0)
					.add(cmdSETNE, AL);
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
				if (isAssignment(op))
					code.add(cmdMOV, ECX, EAX)
						.add(cmdMOV, makeArg(EAX), makeIndirectArg(ECX));
			}
		}
		code.add(cmdPUSH, EAX);
	}
}

void BinaryOpNode::generateLvalue(AsmCode& code) const
{
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	if (op == DOT || op == ARROW) 
	{
		if (op == DOT)
			left->generateLvalue(code);
		else
			left->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdMOV, EBX, dynamic_cast<IdentifierNode*>(right)->sym->offset)
			.add(cmdADD, EAX, EBX)
			.add(cmdPUSH, EAX);
	} else if (isAssignment(op)) {
		generate(code);
		code.add(cmdPOP, EAX);
		left->generateLvalue(code);
	} else 
		throw CompilerException("not implemented", token->line, token->col);
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

string FloatNode::constName() const
{
	return "float" + to_string(index);
}

void FloatNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<FloatToken*>(token)->val << endl;
}

void FloatNode::generate(AsmCode& code) const
{
	code.add(cmdPUSH, makeArgMemory(constName()));
}

void FloatNode::generateLvalue(AsmCode& code) const
{
	code.add(cmdPUSH, makeArgMemory("offset " + constName()));
}

void FloatNode::generateData(AsmCode& code) const
{
	code.add(cmdDD, makeArgMemory(constName()), makeFloat(dynamic_cast<FloatToken*>(token)->val));
}

void FloatNode::generateLoadInFPUStack(AsmCode& code) const
{
	code.add(cmdFLD, makeArgMemory(constName()));
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
	int size = sym->byteSize();
	int steps = size / 4 + (size % 4 != 0);
	if (sym->global)
		for (int i = 0; i < steps; i++)
			code.add(cmdPUSH, makeArgMemory("dword ptr [var_" + sym->name + " + " + to_string(4 * (steps - i - 1)) +"]"));
	else
		for (int i = 0; i < steps; i++)
			code.add(cmdPUSH, makeIndirectArg(EBP, sym->offset + 4 * (steps - i - 1)));
}

void IdentifierNode::generateLvalue(AsmCode& code) const
{
	if (sym->global)
		code.add(cmdPUSH, makeArgMemory("offset var_" + sym->name));
	else 
		code.add(cmdMOV, EAX, EBP)
			.add(cmdMOV, EBX, sym->offset)
			.add(cmdADD, EAX, EBX)
			.add(cmdPUSH, EAX);
}

void IdentifierNode::generateLoadInFPUStack(AsmCode& code) const
{
	if (sym->global)
		code.add(cmdFLD, makeArgMemory("var_" + sym->name));
	else
		code.add(cmdMOV, makeArg(EAX), makeIndirectArg(EBP, sym->offset))
			.add(cmdMOV, real4, makeArg(EAX))
			.add(cmdFLD, real4);
}

TypeSym* IdentifierNode::getType() const
{
	return sym->getType();
}

bool IdentifierNode::isModifiableLvalue() const 
{
	TypeSym* type = dynamic_cast<VarSym*>(sym)->type;
	return !dynamic_cast<ConstTypeSym*>(type) && !dynamic_cast<FuncSym*>(type) 
		&& !dynamic_cast<StructSym*>(type) 
		&& !dynamic_cast<ArraySym*>(type);
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
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	if (op == BITWISE_AND)
		operand->generateLvalue(code);
	else if (op == INC || op == DEC) {
		operand->generate(code);
		if (*operand->getType() == floatType) 
		{
			generateByteToFPU(code);
			code.add(cmdFLD1)
				.add(op == INC ? cmdFADDP : cmdFSUBP);
			generateST0ToStack(code);			
		} else {
			code.add(cmdPOP, EAX);
			PointerSym* pointer = dynamic_cast<PointerSym*>(operand->getType()); 
			if (pointer)
				code.add(cmdMOV, EBX, pointer->type->byteSize())
					.add(op == INC ? cmdADD : cmdSUB, EAX, EBX);
			else 
				code.add(op == INC ? cmdINC : cmdDEC, EAX);
			code.add(cmdPUSH, EAX);
		}
		operand->generateLvalue(code);
		code.add(cmdPOP, EBX)
			.add(cmdPOP, EAX)
			.add(cmdMOV, makeIndirectArg(EBX), makeArg(EAX))
			.add(cmdPUSH, EAX);
	} else if (op == LOGICAL_NOT) {
		operand->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdCMP, EAX, 0)
			.add(cmdSETE, AL)
			.add(cmdPUSH, EAX);
	} else {
		operand->generate(code);
		code.add(cmdPOP, EAX);
		if (op == MINUS)
			if (*operand->getType() == floatType)
			{
				code.add(cmdPUSH, EAX);
				generateByteToFPU(code);
				code.add(cmdFCHS);
				generateST0ToStack(code);
			} else
				code.add(cmdNEG, EAX)
					.add(cmdPUSH, EAX);
		else if (op == BITWISE_NOT)
			code.add(cmdNOT, EAX)
				.add(cmdPUSH, EAX);
		else if (op == MULT) {
			int size = operand->getType()->nextType()->byteSize();
			int steps = size / 4 + (size % 4 != 0);
			code.add(cmdMOV, EBX, EAX);
			for (int i = 0; i < steps; i++)
				code.add(cmdMOV, makeArg(EAX), makeIndirectArg(EBX, (steps - i - 1) * 4))
					.add(cmdPUSH, EAX);
		}
	}
}

void UnaryOpNode::generateLvalue(AsmCode& code) const
{
	OperationsT op = dynamic_cast<OpToken*>(token)->val;
	switch (op)
	{
	case MULT:
		operand->generate(code);
		break;
	default:
		throw CompilerException("not implemented", 0, 0);
	}
}

void UnaryOpNode::generateLoadInFPUStack(AsmCode& code) const
{
	generate(code);
	generateByteToFPU(code);
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
	operand->generate(code);
	TypeSym* opType = operand->getType();
	if (*type == floatType && *opType == intType)
	{
		code.add(cmdPOP, EAX)
			.add(cmdMOV, real4, makeArg(EAX))
			.add(cmdFILD, real4);
		generateST0ToStack(code);
	} else if (*type == intType && *opType == floatType) {
		generateByteToFPU(code);
		code.add(cmdFISTP, real4)
			.add(cmdPUSH, real4);
	}
}

void CoerceNode::generateLoadInFPUStack(AsmCode& code) const
{
	generate(code);
	generateByteToFPU(code);
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

void FunctionalNode::generateLoadInFPUStack(AsmCode& code) const
{
	generate(code);
	generateByteToFPU(code);
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
	code.add(cmdSUB, ESP, symbol->val->byteSize());	
	for (int i = args.size() - 1; i > -1; i--)
		args[i]->generate(code);
	code.add(cmdCALL, makeLabel("f_" + name->token->text))
		.add(cmdADD, ESP, symbol->params->byteSize());
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
		if (dynamic_cast<AliasSym*>(realParamType))
			realParamType = realParamType->getType();
		if (dynamic_cast<AliasSym*>(formalParamType))
			formalParamType = formalParamType->getType();
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
	generateLvalue(code);
	code.add(cmdPOP, EAX)
		.add(cmdPUSH, makeIndirectArg(EAX));
}

void ArrNode::generateLvalue(AsmCode& code) const
{
	TypeSym* nameType = name->getType();
	if (dynamic_cast<ArraySym*>(nameType))
		name->generateLvalue(code);
	else if (dynamic_cast<PointerSym*>(nameType))
		name->generate(code);
	TypeSym* type = nameType->nextType();
	for (int i = 0; i < args.size(); i++)
	{
		args[i]->generate(code);
		code.add(cmdPOP, EAX)
			.add(cmdMOV, EBX, type->byteSize())
			.add(cmdIMUL, EAX, EBX)
			.add(cmdPOP, EBX)
			.add(cmdADD, EAX, EBX)
			.add(cmdPUSH, EAX);
		type = type->nextType();
	}
}

TypeSym* ArrNode::getType() const
{
	TypeSym* sym = dynamic_cast<ArraySym*>(name->getType());
	if (!sym)
		sym = dynamic_cast<PointerSym*>(name->getType());
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
	int size = 0;
	for (int i = args.size() - 1; i > -1; i--)
	{
		TypeSym* type = args[i]->getType();
		args[i]->generate(code);
		size += type->byteSize();
		if (*type == floatType)
		{
			code.add(cmdPOP, real4)
				.add(cmdFLD, real4)
				.add(cmdFSTP, real8)
				.add(cmdMOV, makeArg(EAX), makeArgMemory(real8name, true))
				.add(cmdPUSH, makeIndirectArg(EAX, 4))
				.add(cmdPUSH, makeIndirectArg(EAX));
			size += 4;
		}
	}
	code.add(token->val, makeArgMemory("str" + to_string(format->index)));
	code.add(cmdADD, ESP, size);
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

void StringNode::generateData(AsmCode& code) const
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

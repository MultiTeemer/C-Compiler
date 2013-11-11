#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <map>
#include <stack>
#include "Scanner.h"
#include "Symbols.h"

using namespace std;

class Parser
{
private:
	Scanner lexer;	
	SymTableStack tableStack;
	stack<Block*> blocks;
	map<OperationsT, int> priorityTable;
	map<OperationsT, bool> unaryOps;
	map<OperationsT, bool> rightAssocOps;		
	Symbol* parseComplexDecl(TypeSym* baseType);
	Symbol* parseIdentifier(TypeSym* baseType);
	VarSym* parseDirectDecl();
	TypeSym* parseType(bool inParamList = false);
	TypeSym* parseArrayDimensions(TypeSym* baseType, bool inParamList = false);
	StructSym* parseStruct(bool inParamList);
	FuncSym* createFunctionSymbol(const string& name, TypeSym* type);
	Node* parseFactor();
	Node* parseExpressionTree(int priority = 0);
	Statement* parseStatement();
	JumpStatement* parseJumpStatement();
	Expression* fetchCondition();
	ForStatement* parseFor();
	WhilePreCondStatement* parseWhile();
	WhilePostCondStatement* parseDoWhile();
	IfStatement* parseIf();
	Block* parseBlock();		
	void hitch(VarSym* start, TypeSym* type);
	void parseFuncCall(NodeP& root);
	void parseArrIndex(NodeP& root);
	void parseDeclaration();
	void parseTypeDef();
	void throwException(bool condition, const char* msg);
	void initBlock();
	void popBlock();
	void parseArgList();
	void parseParam();
public:
	Parser(Scanner& scanner);
	Expression* parseExpression(int priority = 0);	
	void parse();
};

#endif
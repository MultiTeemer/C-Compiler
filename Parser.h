#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <map>
#include <stack>
#include "Scanner.h"
#include "Nodes.h"
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
	Node* parseFactor();
	void parseFuncCall(NodeP& root);
	void parseArrIndex(NodeP& root);
	Symbol* parseParam(SymTable* nameSpace);
	Symbol* parseIdentifier(TypeSym* baseType);
	TypeSym* parseType(bool inParamList = false);
	TypeSym* parseArrayDimensions(TypeSym* baseType, bool inParamList = false);
	StructSym* parseStruct(bool inParamList);
	void parseDeclaration();
	void parseTypeDef();
	void throwException(bool condition, const char* msg);
	void initBlock();
	void popBlock();
	Node* parseStatement();
	Node* parseJumpStatement();
	Node* fetchCondition();
	ForStatement* parseFor();
	WhilePreCondStatement* parseWhile();
	WhilePostCondStatement* parseDoWhile();
	IfStatement* parseIf();
	Block* parseBlock();
public:
	Parser(Scanner& scanner);
	Node* parseExpression(int priority = 0);	
	void parse();
};

#endif
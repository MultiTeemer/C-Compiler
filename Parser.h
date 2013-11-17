#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <map>
#include <stack>
#include <vector>
#include "Scanner.h"
#include "Symbols.h"

using namespace std;

class Parser
{
private:
	int nameCounter;
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
	Node* parseMember(Node* left);
	Statement* parseStatement();
	JumpStatement* parseJumpStatement();
	Node* fetchCondition();
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
	Node* parseExpression(int priority = 0);	
	void parse();
};

#endif
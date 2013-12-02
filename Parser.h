#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <map>
#include <stack>
#include <vector>
#include "Scanner.h"
#include "Symbols.h"
#include "CodeGenerator.h"

using namespace std;

class Parser
{
private:
	int nameCounter;
	Scanner lexer;		
	CodeGenerator generator;
	SymTableStack tableStack;
	stack<Block*> blocks;
	map<OperationsT, int> priorityTable;
	map<OperationsT, bool> unaryOps;
	map<OperationsT, bool> rightAssocOps;
	vector<StringNode*> stringConsts;
	VarSym* parseComplexDecl(TypeSym* baseType);
	VarSym* parseIdentifier(TypeSym* baseType);
	VarSym* parseDirectDecl();
	TypeSym* parseType(bool inParamList = false);
	TypeSym* parseArrayDimensions(TypeSym* baseType, bool inParamList = false);
	StructSym* parseStruct(bool inParamList);
	FuncSym* createFunctionSymbol(TypeSym* type);
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
	Parser(Scanner& scanner, CodeGenerator& codeGen);
	Node* parseExpression(int priority = 0);	
	void parse();
	void print() const;
	void generateCode();
};

#endif
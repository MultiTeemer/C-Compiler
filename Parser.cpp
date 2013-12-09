#include <iostream>
#include "Parser.h"

using namespace std;

Parser::Parser(Scanner& scanner, CodeGenerator& codeGen): lexer(scanner), generator(codeGen), nameCounter(0),
	stringConsts(0)
{ 
	lexer.next(); 
	
	priorityTable[PARENTHESIS_FRONT] = 15;
	priorityTable[BRACKET_FRONT] = 15;
	priorityTable[ARROW] = 15;
	priorityTable[DOT] = 15;

	priorityTable[LOGICAL_NOT] = 14;
	priorityTable[BITWISE_NOT] = 14;
	priorityTable[INC] = 14;
	priorityTable[DEC] = 14;

	priorityTable[MULT] = 13;
	priorityTable[DIV] = 13;
	priorityTable[MOD] = 13;

	priorityTable[PLUS] = 12;
	priorityTable[MINUS] = 12;

	priorityTable[BITWISE_SHIFT_LEFT] = 11;
	priorityTable[BITWISE_SHIFT_RIGHT] = 11;

	priorityTable[LESS] = 10;
	priorityTable[LESS_OR_EQUAL] = 10;
	priorityTable[GREATER] = 10;
	priorityTable[GREATER_OR_EQUAL] = 10;

	priorityTable[EQUAL] = 9;
	priorityTable[NOT_EQUAL] = 9;

	priorityTable[BITWISE_AND] = 8;
	
	priorityTable[BITWISE_XOR] = 7;

	priorityTable[BITWISE_OR] = 6;

	priorityTable[LOGICAL_AND] = 5;

	priorityTable[LOGICAL_OR] = 4;

	priorityTable[QUESTION] = 3;
	priorityTable[COLON] = 0; 

	priorityTable[ASSIGN] = 2;
	priorityTable[PLUS_ASSIGN] = 2;
	priorityTable[MINUS_ASSIGN] = 2;
	priorityTable[MULT_ASSIGN] = 2;
	priorityTable[DIV_ASSIGN] = 2;
	priorityTable[MOD_ASSIGN] = 2;
	priorityTable[AND_ASSIGN] = 2;
	priorityTable[OR_ASSIGN] = 2;
	priorityTable[XOR_ASSIGN] = 2;
	priorityTable[AND_ASSIGN] = 2;
	priorityTable[OR_ASSIGN] = 2;
	priorityTable[BITWISE_SHIFT_LEFT_ASSIGN] = 2;
	priorityTable[BITWISE_SHIFT_RIGHT_ASSIGN] = 2;

	priorityTable[COMMA] = 1;

	unaryOps[LOGICAL_NOT] = true;
	unaryOps[MULT] = true; 
	unaryOps[INC] = true;
	unaryOps[DEC] = true;
	unaryOps[BITWISE_AND] = true; 
	unaryOps[PLUS] = true;
	unaryOps[MINUS] = true;
	unaryOps[BITWISE_NOT] = true;

	rightAssocOps[ASSIGN] = true;
	rightAssocOps[PLUS_ASSIGN] = true;
	rightAssocOps[MINUS_ASSIGN] = true;
	rightAssocOps[MULT_ASSIGN] = true;
	rightAssocOps[DIV_ASSIGN] = true;
	rightAssocOps[MOD_ASSIGN] = true;
	rightAssocOps[AND_ASSIGN] = true;
	rightAssocOps[OR_ASSIGN] = true;
	rightAssocOps[XOR_ASSIGN] = true;
	rightAssocOps[BITWISE_SHIFT_LEFT_ASSIGN] = true;
	rightAssocOps[BITWISE_SHIFT_RIGHT_ASSIGN] = true;
	rightAssocOps[LOGICAL_NOT] = true;
	rightAssocOps[BITWISE_NOT] = true;
	rightAssocOps[MULT] = true;
	rightAssocOps[BITWISE_AND] = true;
	rightAssocOps[QUESTION] = true;

	SymTable* predefined = new SymTable();
	predefined->add(intType);
	predefined->add(floatType);
	predefined->add(charType);
	predefined->add(voidType);
	tableStack.push(predefined);

	tableStack.push(new SymTable());

	typePriority[charType] = 1;
	typePriority[intType] = 2;
	typePriority[floatType] = 3;

	operationTypeOperands[MOD] = intType;
	operationTypeOperands[BITWISE_AND] = intType;
	operationTypeOperands[BITWISE_OR] = intType;
	operationTypeOperands[BITWISE_XOR] = intType;
	operationTypeOperands[BITWISE_NOT] = intType;
	operationTypeOperands[BITWISE_SHIFT_LEFT] = intType;
	operationTypeOperands[BITWISE_SHIFT_RIGHT] = intType;

	operationReturningType[MOD] = intType;
	operationReturningType[BITWISE_AND] = intType;
	operationReturningType[BITWISE_OR] = intType;
	operationReturningType[BITWISE_NOT] = intType;
	operationReturningType[BITWISE_SHIFT_LEFT] = intType;
	operationReturningType[BITWISE_SHIFT_RIGHT] = intType;
	operationReturningType[LOGICAL_AND] = intType;
	operationReturningType[LOGICAL_OR] = intType;
	operationReturningType[BITWISE_XOR] = intType;
	operationReturningType[LOGICAL_NOT] = intType;
	operationReturningType[BITWISE_XOR] = intType;
	operationReturningType[EQUAL] = intType;
	operationReturningType[NOT_EQUAL] = intType;
	operationReturningType[LESS] = intType;
	operationReturningType[LESS_OR_EQUAL] = intType;
	operationReturningType[GREATER] = intType;
	operationReturningType[GREATER_OR_EQUAL] = intType;
}

void Parser::throwException(bool condition, const char* msg)
{
	if (condition)
		throw ParserException(msg, lexer.get()->line, lexer.get()->col);
}

void Parser::parseFuncCall(NodeP& root)
{
	Token* next = lexer.get();
	if (*next == PARENTHESIS_FRONT)
		{
			Token* t = lexer.next();
			root = new FuncCallNode(root->token, root, dynamic_cast<FuncSym*>(root->getType()));
			while (*t != PARENTHESIS_BACK)
			{
				dynamic_cast<FuncCallNode*>(root)->addArg(parseExpression(priorityTable[COMMA] + 1));
				t = lexer.get();
				throwException(*t == END_OF_FILE, "Expected parenthesis close after function argument list");
				if (*t == COMMA)
					t = lexer.next();
			}
			lexer.next();
		}
}

void Parser::parseArrIndex(NodeP& root)
{
	Token* next = lexer.get();
	if (*next == BRACKET_FRONT)
		{
			root = new ArrNode(root->token, root);
			Token* t = lexer.get();
			while (*t == BRACKET_FRONT)
			{
				lexer.next();
				Node* index = parseExpression();
				dynamic_cast<ArrNode*>(root)->addArg(index);
				t = lexer.get();
				throwException(*t != BRACKET_BACK, "Expected bracket close after array index");
				t = lexer.next();
			}
		}
}

Node* Parser::parseFactor()
{
	Node* root = 0;
	bool nextNeeded = true;
	Token* token = lexer.get();
	if (*token == SEMICOLON)
		return new EmptyNode();
	switch (token->type) {
	case INTEGER:
		root = new IntNode(token);
		parseArrIndex(root);
		break;
	case REAL_NUMBER:
		root = new FloatNode(token);
		break;
	case IDENTIFIER:
		{
			Symbol* sym = tableStack.find(token->text);
			throwException(!sym, "Undefined name");
			throwException(!dynamic_cast<VarSym*>(sym), "???");
			root = new IdentifierNode(token, dynamic_cast<VarSym*>(sym));
			parseFuncCall(root);
		}
		break; 
	case CHARACTER:
		root = new CharNode(token);
		break;
	case STRING:
		{
			StringNode* string = new StringNode(token, stringConsts.size());
			stringConsts.push_back(string);
			root = string;
			break;
		}
	case KEYWORD:
		{
			KeywordsT kw = dynamic_cast<KeywordToken*>(token)->val;
			if (kw == CHAR || kw == INT || kw == FLOAT || kw == SIZEOF)
			{
				lexer.next();
				string typeName = kw == CHAR ? "char" : kw == INT ? "int" : "float";
				throwException(*lexer.get() != PARENTHESIS_FRONT, "Expected open parenthesis");
				lexer.next();
				root = new CoerceNode(token, parseExpression(), dynamic_cast<TypeSym*>(tableStack.find(typeName)));
				throwException(*lexer.get() != PARENTHESIS_BACK, "Expected close parenthesis");
				lexer.next();
			} else 
				throwException(true, "you can use keywords just like a char, int, float, sizeof in expressions");
			nextNeeded = false;
			break;
		}
	case OPERATION:
		{
			if (*token == PRINTF || *token == SCANF)
			{
				throwException(*lexer.next() != PARENTHESIS_FRONT, "Expected open parenthesis");
				lexer.next();
				StringNode* format = dynamic_cast<StringNode*>(parseExpression(priorityTable[COMMA] + 1));
				throwException(!format, "Expected format string");
				IOOperatorNode* node = new IOOperatorNode(dynamic_cast<OpToken*>(token), format);
				if (*lexer.get() == COMMA)
				{
					lexer.next();
					while (1)
					{
						Node* arg = parseExpression(priorityTable[COMMA] + 1);
						throwException(!arg, "Expected argument");
						node->addArg(arg);
						if (*lexer.get() == PARENTHESIS_BACK)
							break;
						if (*lexer.get() == COMMA)
							lexer.next();
					}
				}
				*lexer.next();
				nextNeeded = false;
				root = node;
			} else if (*token == PARENTHESIS_FRONT)	{
				lexer.next();
				root = parseExpression();
				Token* close = lexer.get();
				if (!close || *close != PARENTHESIS_BACK)
					throw ParserException("Expected parenthesis close", root->token->line, root->token->col);
			} else if (unaryOps[dynamic_cast<OpToken*>(token)->val] == true) {
				lexer.next();
				root = new UnaryOpNode(token, parseExpression(priorityTable[DEC]));
				nextNeeded = false;
			} else
				throwException(true, "Empty expression is not allowed");
		}
		break;
	}
	if (nextNeeded)
		lexer.next();
	return root;
}

Node* Parser::parseMember(Node* left)
{
	TypeSym* type = left->getType();
	StructSym* structType = 0;
	if (dynamic_cast<PointerSym*>(type)) 
		structType = dynamic_cast<StructSym*>(dynamic_cast<PointerSym*>(type)->type);
	else 
		structType= dynamic_cast<StructSym*>(type);
	throwException(!structType, "Left operand of . or -> must be a structure");
	Token* opTok = lexer.get();
	Token* token = lexer.next();
	throwException(!dynamic_cast<IdentifierToken*>(token), "Right operand of . or -> must be a identifier");
	string fieldName = token->text;
	if (!structType->fields->exists(fieldName))
		fieldName = '$' + fieldName;
	throwException(!structType->fields->exists(fieldName), "Undefined field in structure");
	lexer.next();
	Node* right = new IdentifierNode(token, dynamic_cast<VarSym*>(structType->fields->find(fieldName)));
	return new BinaryOpNode(opTok, left, right);
}

Node* Parser::parseExpression(int priority)
{
	if (priority > 15)
		return parseFactor();
	Node* left = parseExpression(priority + 1);
	Node* root = left;
	Token* opTok = lexer.get();
	if (*opTok == END_OF_FILE || *opTok == PARENTHESIS_BACK || *opTok == BRACKET_BACK
		|| *opTok == COLON || *opTok == SEPARATOR)
	{
		root->getType();
		return root;
	}
	throwException(!dynamic_cast<OpToken*>(opTok), "Invalid expression. Expected operation");
	if (priorityTable[dynamic_cast<OpToken*>(opTok)->val] < priority)
		return root;
	while (dynamic_cast<OpToken*>(opTok) && priorityTable[dynamic_cast<OpToken*>(opTok)->val] >= priority)
	{
		OperationsT op = dynamic_cast<OpToken*>(opTok)->val;	
		if (op == PARENTHESIS_FRONT)
			parseFuncCall(root);
		else if (op == BRACKET_FRONT)
			parseArrIndex(root);
		else if (op == INC || op == DEC) {
			root = new PostfixUnaryOpNode(opTok, root);
			lexer.next();		
		} else if (op == QUESTION) {
			lexer.next();
			Node* l = parseExpression();
			if (*lexer.get() != COLON)
				throw ParserException("Missed branch of ternary operator", lexer.line, lexer.col);
			lexer.next();
			Node* r = parseExpression();
			root = new TernaryOpNode(opTok, root, l, r);
		} else if (op == DOT || op == ARROW)
			root = parseMember(root);		
		else {
			lexer.next();
			root = new BinaryOpNode(opTok, root, parseExpression(priority + (rightAssocOps[op] == true ? 0 : 1)));		
		}
		opTok = lexer.get();
	}
	root->getType();
	return root;
}

StructSym* Parser::parseStruct(bool inParamList)
{
	Token* token = lexer.next();
	string structName;
	if (dynamic_cast<IdentifierToken*>(token)) 
	{
		structName = token->text;
		token = lexer.next();
	} else 
		structName = "$$unnamedStruct" + to_string(nameCounter++);	
	StructSym* structType = dynamic_cast<StructSym*>(tableStack.find(structName));
	throwException(inParamList && !structType, "Unknown struct type");
	if (!structType)	
	{
		structType = new StructSym(structName, 0);
		if (structName.length() > 0)
			tableStack.add(structType);
	}
	if (*token == BRACE_FRONT)
	{
		throwException(inParamList, "Type definition is not allowed");
		throwException(structType->fields, "Struct redefinition");
		structType->fields = new SymTable();
		tableStack.push(structType->fields);
		token = lexer.next();
		while (*token != BRACE_BACK)
		{
			TypeSym* type = parseType();
			while (*lexer.get() != SEMICOLON)
			{
				structType->fields->add(parseIdentifier(type));
				token = lexer.get();
				if (*token == COMMA)
					token = lexer.next();
			}
			token = lexer.next();
		}
		tableStack.pop();
		lexer.next();
	}
	return structType;	
}

TypeSym* Parser::parseType(bool inParamList)
{
	bool isConst = false;
	if (*lexer.get() == CONST)
	{
		isConst = true;
		lexer.next();
	}
	Token* token = lexer.get();
	TypeSym* type = 0;
	if (*token == STRUCT) 
		type = parseStruct(inParamList);
	else {
		type = dynamic_cast<TypeSym*>(tableStack.find(token->text));
		lexer.next();
	}
	throwException(!type, "Unknown type");		
	if (isConst)
		type = new ConstTypeSym(type);
	return type;
}

TypeSym* Parser::parseArrayDimensions(TypeSym* baseType, bool inParamList)
{
	Token* token = lexer.get();
	vector<int> arrSizes;
	while (*token == BRACKET_FRONT)
	{
		token = lexer.next();
		int size = dynamic_cast<IntegerToken*>(token) ? dynamic_cast<IntegerToken*>(token)->val : -1;
		throwException(size == 0 || size < -1, "Array bounds must be a positive integer");
		arrSizes.push_back(size);
		if (size != -1)
			token = lexer.next();
		throwException(*token != BRACKET_BACK, "Expected bracket close after array bounds");
		token = lexer.next();
	}
	for (int i = 1; i < arrSizes.size(); i++)
		throwException(arrSizes[i] == -1, "Expected array bounds");
	throwException(!inParamList && arrSizes.size() == 1 && arrSizes[0] == -1, "Expected array bounds");
	for (int i = arrSizes.size() - 1; i >= 0; i--)
		baseType = new ArraySym(baseType, arrSizes[i]);
	return baseType;
}

void Parser::parseParam()
{
	Symbol* param = 0;
	TypeSym* type = parseType(true);
	Token* token = lexer.get();
	throwException(*type == "void" && *token != MULT, "Argument type cannot be a void");	
	while (*token == MULT)
	{
		type = new PointerSym(type);
		token = lexer.next();
	}	
	if (*token == PARENTHESIS_FRONT)
		param = parseComplexDecl(type);
	else {
		string name = dynamic_cast<IdentifierToken*>(token) ? token->text : "";
		if (name.length() > 0) 
		{
			throwException(tableStack.top()->exists(name), "Redefinition");
			token = lexer.next();
		}
		if (*token == BRACKET_FRONT)
			type = parseArrayDimensions(type, true);
		param = new VarSym(name, type);
	}
	tableStack.add(param);
}

void Parser::parseArgList()
{
	Token* token = lexer.next();
	while (*token != PARENTHESIS_BACK)
	{
		parseParam();
		token = lexer.get();
		throwException(*token != COMMA && *token != PARENTHESIS_BACK, "Expected comma or close parenthesis");
		if (*token == COMMA)
			token = lexer.next();
	}
	lexer.next();
}

FuncSym* Parser::createFunctionSymbol(TypeSym* type)
{
	FuncSym* function = new FuncSym(type);
	function->params = new SymTable();
	tableStack.push(function->params);
	parseArgList();
	tableStack.pop();
	return function;
}

VarSym* Parser::parseIdentifier(TypeSym* type)
{
	VarSym* res = 0;
	Token* token = lexer.get();
	while (*token == MULT)
	{
		type = new PointerSym(type);
		token = lexer.next();
	}
	if (*token == PARENTHESIS_FRONT)
		return parseComplexDecl(type);
	throwException(!dynamic_cast<IdentifierToken*>(token), "Expected identifier");
	string name = dynamic_cast<IdentifierToken*>(token)->val;
	token = lexer.next();	
	if (*token != PARENTHESIS_FRONT)
	{
		if (*token == BRACKET_FRONT)
			type = parseArrayDimensions(type);
		if (type->isStruct())
			name = '$' + name;
	} else 		
		type = createFunctionSymbol(type);	
	res = new VarSym(name, type);
	throwException(tableStack.existsInLastNamespace(name), "Redefinition");
	return res;
}

void Parser::hitch(VarSym* start, TypeSym* type)
{
	if (!start->type)
		start->type = type;
	else {
		TypeSym* penultimate = start->type;
		while (penultimate->nextType())
			penultimate = penultimate->nextType();
		penultimate->setNextType(type);
	}
}

VarSym* Parser::parseDirectDecl()
{
	VarSym* sym = 0;
	TypeSym* type = 0;
	while (*lexer.next() == MULT)
		type = new PointerSym(type);
	if (*lexer.get() == PARENTHESIS_FRONT)
		sym = parseDirectDecl();
	else {
		throwException(!dynamic_cast<IdentifierToken*>(lexer.get()), "Expected identifier");
		throwException(tableStack.top()->exists(lexer.get()->text), "Redefinition");
		sym = new VarSym(lexer.get()->text, 0);
		lexer.next();
	}
	if (*lexer.get() == PARENTHESIS_FRONT)	
		type = createFunctionSymbol(type);
	 else if (*lexer.get() == BRACKET_FRONT) 
		type = parseArrayDimensions(type, true);	
	hitch(sym, type);
	lexer.next();
	return sym;
}

VarSym* Parser::parseComplexDecl(TypeSym* baseType)
{
	VarSym* sym = parseDirectDecl();
	if (*lexer.get() == PARENTHESIS_FRONT)
		baseType = createFunctionSymbol(baseType);
	else if (*lexer.get() == BRACKET_FRONT)
		baseType = parseArrayDimensions(baseType, true);
	hitch(sym, baseType);
	return sym;
}

void Parser::parseDeclaration()
{	
	TypeSym* type = parseType();
	VarSym* sym = 0;
	while (true)
	{
		if (type->isStruct() && *lexer.get() == SEMICOLON)
			break;
		Token* token = lexer.get();
		sym = *lexer.get() == PARENTHESIS_FRONT ? parseComplexDecl(type) : parseIdentifier(type);		
		tableStack.add(sym);
		if (*lexer.get() == ASSIGN)
		{
			Token* assign = lexer.get();
			lexer.next();
			Node* assignOperand = parseExpression(priorityTable[COMMA] + 1);
			throwException(blocks.size() == 0, "Cannot assign out of block");
			BinaryOpNode* node = new BinaryOpNode(assign, new IdentifierNode(token, sym), assignOperand);
			node->getType();
			blocks.top()->AddStatement(new SingleStatement(node));
		}
		if (*lexer.get() == SEMICOLON || *lexer.get() == BRACE_FRONT)
			break;
		else if (*lexer.get() != COMMA)
			throwException(true, "Expected comma");
		else
			lexer.next();
	};
	if (*lexer.get() == BRACE_FRONT)
		if (dynamic_cast<FuncSym*>(sym->type))
		{
			throwException(blocks.size() != 0, "Cannot define function in block");
			dynamic_cast<FuncSym*>(sym->type)->body = parseBlock();
		} else 
			throwException(true, "Unexpected brace");
	lexer.next();
}

JumpStatement* Parser::parseJumpStatement()
{
	JumpStatement* stmnt = 0;
	switch (dynamic_cast<KeywordToken*>(lexer.get())->val) 
	{
	case CONTINUE:
		stmnt = new ContinueStatement();
		break;
	case BREAK:
		stmnt = new BreakStatement();
		break;
	case RETURN:
		Node* arg = *lexer.next() != SEMICOLON ? parseExpression() : 0;
		stmnt = new ReturnStatement(arg);
	}
	if (*lexer.get() != SEMICOLON)
		if (*lexer.next() != SEMICOLON)
			throw ParserException("Expected semicolon", lexer.get()->line, lexer.get()->col);
	lexer.next();
	return stmnt;
}

Statement* Parser::parseStatement() 
{
	Token* token = lexer.get();
	if (*token == IF)
		return parseIf();
	else if (*token == FOR)
		return parseFor();
	else if (*token == WHILE)
		return parseWhile();
	else if (*token == DO)
		return parseDoWhile();
	else if (*token == BRACE_FRONT)	
		return parseBlock();
	else if (*token == CONTINUE || *token == BREAK || *token == RETURN)
		return parseJumpStatement();
	else {
		SingleStatement* stnmt = new SingleStatement(parseExpression());
		if (*lexer.get() == SEMICOLON)
			lexer.next();
		return stnmt;
	}	
}

Node* Parser::fetchCondition()
{
	throwException(*lexer.next() != PARENTHESIS_FRONT, "Expected open parenthesis");
	lexer.next();
	Node* condition = parseExpression();
	throwException(!condition, "Loop must have an condition");
	throwException(*lexer.get() != PARENTHESIS_BACK, "Expected close parenthesis");
	lexer.next();
	return condition;
}

void Parser::initBlock()
{
	Block* block = new Block(new SymTableForLocals());
	blocks.push(block);
	tableStack.push(block->locals);
}

void Parser::popBlock()
{
	blocks.pop();
	tableStack.pop();
}

ForStatement* Parser::parseFor()
{
	throwException(*lexer.next() != PARENTHESIS_FRONT, "Expected open parenthesis");
	lexer.next();
	Node* initialization = parseExpression();
	throwException(*lexer.get() != SEMICOLON, "Expected semicolon");
	lexer.next();
	Node* condition = parseExpression();
	throwException(*lexer.get() != SEMICOLON, "Expected semicolon");
	lexer.next();
	Node* increment = *lexer.get() != PARENTHESIS_BACK ? parseExpression() : new EmptyNode();
	throwException(*lexer.get() != PARENTHESIS_BACK, "Expected close parenthesis");
	lexer.next();
	initBlock();
	Statement* body = parseStatement();
	popBlock();
	return new ForStatement(initialization, condition, increment, body);
}

WhilePreCondStatement* Parser::parseWhile()
{
	Node* condition = fetchCondition();
	initBlock();
	Statement* body = parseStatement();
	popBlock();
	return new WhilePreCondStatement(condition, body);
}

WhilePostCondStatement* Parser::parseDoWhile()
{
	lexer.next();
	initBlock();
	Statement* body = parseStatement();
	popBlock();
	throwException(*lexer.get() != WHILE, "Expected 'while'");
	Node* condition = fetchCondition();
	throwException(*lexer.get() != SEMICOLON, "Expected semicolon");
	lexer.next();
	return new WhilePostCondStatement(condition, body);
}

IfStatement* Parser::parseIf()
{
	Node* condition = fetchCondition();
	initBlock();
	Statement* trueBranch = parseStatement();
	popBlock();
	Statement* falseBranch = 0;
	if (*lexer.get() == ELSE)
	{
		lexer.next();
		initBlock();
		falseBranch = parseStatement();
		popBlock();
	}
	return new IfStatement(condition, trueBranch, falseBranch);
}

Block* Parser::parseBlock()
{
	lexer.next();
	Block* block = new Block(new SymTableForLocals());
	blocks.push(block);
	tableStack.push(block->locals);
	Token* token = lexer.get();
	while (*token != BRACE_BACK)
	{
		if (*token == CONST || *token == STRUCT || dynamic_cast<TypeSym*>(tableStack.find(token->text)))
			parseDeclaration();
		else if (*token == TYPEDEF)
			parseTypeDef();
		else
			block->AddStatement(parseStatement());
		token = lexer.get();
	}
	lexer.next();
	blocks.pop();
	tableStack.pop();
	return block;
}

void Parser::parseTypeDef()
{
	lexer.next();
	TypeSym* type = parseType();
	while (*lexer.get() == MULT)
	{
		type = new PointerSym(type);
		lexer.next();
	}
	while (*lexer.get() != SEMICOLON)
	{
		string alias = lexer.get()->text;
		throwException(tableStack.find(alias), "Alias must be a unique");
		tableStack.add(new AliasSym(alias, type));
		lexer.next();
		if (*lexer.get() == COMMA)
			lexer.next();
	}
	lexer.next();
}

void Parser::parse()
{
	Token* token = lexer.get();
	while (*token != END_OF_FILE)
	{
		if (*token == CONST || *token == STRUCT || dynamic_cast<TypeSym*>(tableStack.find(token->text)))
			parseDeclaration();
		else if (*token == TYPEDEF)
			parseTypeDef();
		else 
			parseExpression()->print();
		token = lexer.get();
	}
}

void Parser::print() const
{
	tableStack.print();
}


void Parser::generateCode() 
{
	for (int i = 0; i < stringConsts.size(); i++)
		stringConsts[i]->generate(generator.data);
	tableStack.top()->generateGlobals(generator.data);
	tableStack.top()->generateCode(generator.code);
	generator.generate();
}
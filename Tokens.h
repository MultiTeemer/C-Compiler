#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>

using namespace std;

const string lineSeparator("+------------------------------------------------------------------+\n");

static const int resourceSize = 200;

typedef enum {
	IDENTIFIER, 
	KEYWORD,
	INTEGER,
	REAL_NUMBER,
	STRING,
	CHARACTER,
	OPERATION,
	SEPARATOR,
	WHITESPACE,
	COMMENT,
	END_OF_FILE,
	PREPROCESSOR_DIRECTIVE
} TokenTypesT;

typedef enum {
	BREAK, 
	CHAR,
	CONST,
	CONTINUE,
	DO,
	ELSE,
	FLOAT,
	FOR,
	IF,
	INT,
	RETURN,
	STRUCT,
	SIZEOF,
	TYPEDEF,
	VOID,
	WHILE
} KeywordsT;

typedef enum {
	BRACE_FRONT,
	BRACE_BACK,
	SEMICOLON,
	SEPARATORS_COUNT
} SeparatorsT;

typedef enum {
	PLUS, 
	MINUS,
	MULT,
	DIV,
	MOD,
	INC,
	DEC,
	PLUS_ASSIGN,
	MINUS_ASSIGN,
	MULT_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	QUESTION,
	COLON,
	BITWISE_AND,
	BITWISE_OR,
	LOGICAL_AND,
	LOGICAL_OR,
	LOGICAL_NOT,
	BITWISE_XOR,
	AND_ASSIGN,
	OR_ASSIGN,
	XOR_ASSIGN,
	ASSIGN,
	EQUAL,
	NOT_EQUAL,
	GREATER,
	LESS,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	BITWISE_SHIFT_LEFT,
	BITWISE_SHIFT_RIGHT,
	DOT,
	ARROW,
	BITWISE_SHIFT_LEFT_ASSIGN,
	BITWISE_SHIFT_RIGHT_ASSIGN,
	PARENTHESIS_FRONT,
	PARENTHESIS_BACK,
	BRACKET_FRONT,
	BRACKET_BACK,
	COMMA,
	BITWISE_NOT,
	OPERATIONS_COUNT,
	PRINTF,
	SCANF
} OperationsT;

class Token
{
protected:	
	TokenTypesT type;	
	string tokenTypeName(TokenTypesT type);	
public:	
	int line;
	int col;
	string text;
	friend class Parser;	
	friend class BinaryOpNode;
	friend class UnaryOpNode;
	friend class KeywordNode;
	Token(TokenTypesT t, int l, int c, const string& str): type(t), line(l), col(c), text(str) {}
	bool operator == (TokenTypesT o) const { return type == o; }
	bool operator != (TokenTypesT o) const { return type != o; }
	virtual bool operator == (OperationsT o) const { return false; }
	virtual bool operator != (OperationsT o) const { return true; }
	virtual bool operator == (SeparatorsT o) const { return false; }
	virtual bool operator != (SeparatorsT o) const { return true; }	
	virtual bool operator == (string o) const { return false; }
	virtual bool operator == (KeywordsT o) const { return false; }
	virtual bool operator != (KeywordsT o) const { return true; }
	virtual string info();
};

class EOFToken : public Token
{
public:
	EOFToken(int l, int c): Token(END_OF_FILE, l, c, "") {}
	string info() { return string(); }
};

class StringValToken : public Token
{
protected:
	string val;
public:
	friend class Parser;
	friend class IdentifierNode;
	StringValToken(TokenTypesT t, int l, int c, const string& str): Token(t, l, c, str), val(str) {}
	string info();
};

class StringToken : public StringValToken
{
public:
	friend class Parser;
	friend class StringNode;
	StringToken(TokenTypesT t, int l, int c, const string& str, const string& v): StringValToken(t, l, c, str) 
	{
		val = string(v);
	}
};

class IdentifierToken : public StringValToken
{
public:
	friend class Parser;
	IdentifierToken(int l, int c, const string& str): StringValToken(IDENTIFIER, l, c, str) {}
};

class IntegerToken : public Token
{
private:
	int val;
public:
	friend class Parser;
	friend class IntNode;
	IntegerToken(int l, int c, const string& str, int v): Token(INTEGER, l, c, str), val(v) {}
	string info();
};

class FloatToken : public Token
{
private:
	float val;
public:
	friend class Parser;
	friend class FloatNode;
	FloatToken(int l, int c, const string& str, float v): Token(REAL_NUMBER, l, c, str), val(v) {}
	string info();
};

class CharToken : public Token 
{
private:
	char val;
public:
	friend class Parser;
	friend class CharNode;
	CharToken(int l, int c, const string& str, char v): Token(CHARACTER, l, c, str), val(v) {}
	string info();
};

class OpToken : public Token
{
public:
	friend class IOOperatorNode;
	OperationsT val;
	OpToken(int l, int c, const string& str, OperationsT op): Token(OPERATION, l, c, str), val(op) {}
	bool operator == (OperationsT o) const;
	bool operator != (OperationsT o) const;
};

class SepToken : public Token
{
private:
	SeparatorsT val;
public:
	friend class Parser;
	SepToken(int l, int c, const string& str, SeparatorsT sep): Token(SEPARATOR, l, c, str), val(sep) {}
	bool operator == (SeparatorsT o) const { return o == val; }
	bool operator != (SeparatorsT o) const { return o != val; }
};

class KeywordToken : public Token
{
private:
	KeywordsT val;
public:
	friend class Parser;
	friend class KeywordNode;
	KeywordToken(int l, int c, const string& str, KeywordsT keyword): Token(KEYWORD, l, c, str), val(keyword) {}
	bool operator == (KeywordsT o) const;
	bool operator != (KeywordsT o) const;
};

#endif
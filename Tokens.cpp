#include "Tokens.h"

string Token::tokenTypeName(TokenTypesT type)
{
	switch(type) 
	{
	case IDENTIFIER:
		return "Identifier";
	case KEYWORD: 
		return "Keyword";
	case INTEGER:
		return "Integer";
	case REAL_NUMBER:
		return "Float";
	case STRING:
		return "String";
	case CHARACTER:
		return "Char";
	case OPERATION:
		return "Operation";
	case SEPARATOR:
		return "Separator";
	case PREPROCESSOR_DIRECTIVE:
		return "Preprocessor directive";
	case COMMENT:
		return "Comment";
	}
}

string Token::info()
{
	char info[500];
	sprintf(info, "%s\t\t%d\t\t%d\t\t%s\t\t%s", tokenTypeName(type).c_str(), line, col, text.c_str(), text.c_str());
	return string(info);
}

string StringValToken::info()
{
	char* info;
	int length = val.length() + text.length() + resourceSize;
	info = (char*) malloc(sizeof(char) * length);
	sprintf(info, "%s\t\t%d\t\t%d\t\t%s\t\t%s", tokenTypeName(type).c_str(), line, col, text.c_str(), val.c_str());
	string res(info);
	free(info);
	return res;
}

string IntegerToken::info()
{
	char* info;
	int length = text.length() * 2 + resourceSize;
	info = (char*) malloc(sizeof(char) * length);
	sprintf(info, "%s\t\t%d\t\t%d\t\t%s\t\t%d", tokenTypeName(type).c_str(), line, col, text.c_str(), val);
	string res(info);
	free(info);
	return res;
}

string FloatToken::info()
{
	char* info;
	int length = text.length() * 2 + resourceSize;
	info = (char*) malloc(sizeof(char) * length);
	sprintf(info, "%s\t\t%d\t\t%d\t\t%s\t\t%f", tokenTypeName(type).c_str(), line, col, text.c_str(), val);
	string res(info);
	free(info);
	return res;
}

string CharToken::info()
{
	char* info;
	int length = text.length() * 2 + resourceSize;
	info = (char*) malloc(sizeof(char) * length);
	sprintf(info, "%s\t\t%d\t\t%d\t\t%s\t\t%c", tokenTypeName(type).c_str(), line, col, text.c_str(), val);
	string res(info);
	free(info);
	return res;
}

bool OpToken::operator==(OperationsT o) const 
{
	return o == val;
}

bool OpToken::operator!=(OperationsT o) const
{
	return o != val;
}

bool KeywordToken::operator==(KeywordsT o) const 
{
	return o == val;
}

bool KeywordToken::operator!=(KeywordsT o) const
{
	return o != val;
}

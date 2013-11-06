#include "Exceptions.h"

using namespace std;

string CompilerException::message() const
{
	string msg;
	msg += exception::what();
	msg += "\n";
	msg += "On line ";
	msg += to_string(line);
	msg += " col ";
	msg += to_string(col);
	return msg;
}

const char* CompilerException::what() const
{
	string msg = message();
	char* str = (char*) malloc(sizeof(char) * (msg.length() + 1)); 
	strcpy(str, msg.c_str());
	return str;
}
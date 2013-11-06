#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

using namespace std;

class CompilerException : public exception
{
protected:
	int line;
	int col;
	string message() const;
public:	
	CompilerException(const char* msg, int l, int c): exception(msg), line(l), col(c) {}
	const char* what() const;
};

class ScannerException : public CompilerException
{
public:
	ScannerException(const char* msg, int l, int c): CompilerException(msg, l, c) {}
};

class ParserException : public CompilerException
{
public:
	ParserException(const char* msg, int l, int c): CompilerException(msg, l, c) {}
};

#endif
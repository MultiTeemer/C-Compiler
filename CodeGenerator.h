#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include <vector>
#include "Commands.h"

using namespace std;

class CodeGenerator
{
private:
	string filename;
	AsmCode data;
	AsmCode code;
public:
	friend class Parser;
	CodeGenerator(const string& file): filename(file) {}
	void generate() const;
};

#endif
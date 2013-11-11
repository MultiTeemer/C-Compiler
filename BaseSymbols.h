#ifndef BASE_SYMBOLS_H
#define BASE_SYMBOLS_H

#include <string>

using namespace std;

class Symbol
{
public:
	string name;
	Symbol(const string& n): name(n) {}
	virtual void print(int deep) const;
};


#endif
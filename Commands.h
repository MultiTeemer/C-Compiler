#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include "Tokens.h"

using namespace std;

typedef enum {
	cmdDD,
	cmdDB,
	cmdDQ,
	cmdMOV,
	cmdPUSH,
	cmdPOP,
	cmdMUL,
	cmdIMUL,
	cmdDIV,
	cmdIDIV,
	cmdADD,
	cmdSUB,
	cmdINC,
	cmdDEC,
	cmdRET,
	cmdNEG,
	cmdCDQ,
	cmdINVOKE,
	cmdSHR,
	cmdSHL,
	cmdAND,
	cmdOR,
	cmdXOR,
	cmdNOT,
	cmdCALL,
} AsmCommandsT;

typedef enum {
	EAX,
	EBX,
	ECX,
	EDX,
	EBP,
	ESP,
	CL,
} AsmRegistersT;

class AsmArg
{
public:
	virtual string generate() const = 0;
};

class AsmArgImmediate : public AsmArg
{
private:
	int value;
public:
	AsmArgImmediate(int v): value(v) {}
	string generate() const { return to_string(value); }
};

class AsmArgString : public AsmArg
{
private:
	string value;
public:
	AsmArgString(const string& str): value(str) {}
	string generate() const { return value; }
};

class AsmArgRegister : public AsmArg
{
protected:
	AsmRegistersT reg;
	string regName() const;
public:
	AsmArgRegister(AsmRegistersT r): reg(r) {}
	string generate() const { return regName(); }
};

class AsmArgIndirect : public AsmArgRegister
{
private:
	int offset;
public:
	AsmArgIndirect(AsmRegistersT r, int shift = 0): AsmArgRegister(r), offset(shift) {}
	string generate() const { return "[" + regName() + " + " + to_string(offset) + "]"; }
};

class AsmArgMemory : public AsmArg
{
private:
	string varName;
public:
	AsmArgMemory(const string& name): varName(name) {}
	string generate() const { return varName; }
};

class AsmArgLabel : public AsmArg
{
private:
	string name;
public:
	AsmArgLabel(const string& n): name(n) {}
	string generate() const { return name; }
};

class AsmArgDup : public AsmArg
{
private:
	int count;
public:
	AsmArgDup(int c): count(c) {}
	string generate() const { return to_string(count) + " dup(0)"; }
};

class AsmInstruction 
{
public:
	virtual string generate() const = 0;
};

class AsmLabel : public AsmInstruction
{
protected:
	AsmArgLabel* label;
public:
	AsmLabel(AsmArgLabel* l): label(l) {}
	virtual string generate() const { return label->generate() + ":"; }
};

class AsmCmd : public AsmInstruction
{
protected:
	AsmCommandsT opCode;
	string cmdName() const;
public:
	AsmCmd() {}
	AsmCmd(AsmCommandsT opcode): opCode(opcode) {}
	inline virtual string generate() const;
};

class AsmCmd1 : public AsmCmd
{
private:
	AsmArg* arg;
public:
	AsmCmd1(AsmCommandsT opcode, AsmArg* a): AsmCmd(opcode), arg(a) {}
	inline string generate() const;
};

class AsmCmd2 : public AsmCmd
{
private:
	AsmArg *arg1, *arg2;
public:
	AsmCmd2(AsmCommandsT opcode, AsmArg* a1, AsmArg* a2): AsmCmd(opcode), arg1(a1), arg2(a2) {}
	inline string generate() const;
};

class AsmIOCmd : public AsmCmd
{
private:
	OperationsT mode;
	AsmArgMemory* format;
	AsmArg* arg;
public:
	AsmIOCmd(OperationsT m, AsmArgMemory* f, AsmArg* a): AsmCmd(cmdINVOKE), mode(m), format(f), arg(a) {}
	string generate() const;
};

AsmArgRegister* makeArg(AsmRegistersT reg);
AsmArgImmediate* makeArg(int val);
AsmArgDup* makeArgDup(int count);
AsmArgMemory* makeArgMemory(const string& varName);
AsmArgIndirect* makeIndirectArg(AsmRegistersT reg, int offset = 0);
AsmArgLabel* makeLabel(const string& name);
AsmArgString* makeString(const string& name);

class AsmCode
{
private:
	vector<AsmInstruction*> commands;
public:
	AsmCode(): commands(0) {}
	void fflush(ofstream& out) const;
	AsmCode& operator << (AsmCmd* command);
	AsmCode& add(AsmCommandsT cmd);
	AsmCode& add(AsmCommandsT cmd, AsmArg* arg);
	AsmCode& add(AsmCommandsT cmd, AsmArg* arg1, AsmArg* arg2);
	AsmCode& add(AsmCommandsT cmd, AsmRegistersT reg);
	AsmCode& add(AsmCommandsT cmd, AsmRegistersT reg1, AsmRegistersT reg2);
	AsmCode& add(AsmCommandsT cmd, AsmRegistersT reg, int val);
	AsmCode& add(AsmArgLabel* label);
	AsmCode& add(OperationsT func, AsmArgMemory* format, AsmArg* arg = 0);
};

#endif
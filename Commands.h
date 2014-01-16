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
	cmdJMP,
	cmdCMP,
	cmdJE,
	cmdJNE,
	cmdSETG,
	cmdSETL,
	cmdSETGE,
	cmdSETLE,
	cmdSETE,
	cmdSETNE,
} AsmCommandsT;

typedef enum {
	EAX,
	EBX,
	ECX,
	EDX,
	EBP,
	ESP,
	CL,
	AL,
	BL,
} AsmRegistersT;

class AsmArg
{
public:
	virtual string generate() const = 0;
	virtual bool operator == (int val) const { return false; }
	virtual bool operator == (AsmRegistersT reg) const { return false; }
	virtual bool operator == (AsmArg* o) const { return false; }
	virtual bool isRegister() const { return false; }
	virtual bool isMemoryLocation() const { return false; }
	bool operator != (AsmArg* o) const { return !(*this == o); }
};

class AsmArgImmediate : public AsmArg
{
private:
	int value;
public:
	AsmArgImmediate(int v): value(v) {}
	string generate() const { return to_string(value); }
	bool operator == (int val) { return value == val; }
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
	bool operator == (AsmArg* o) const;
	bool operator == (AsmRegistersT r) const { return r == reg; }
	bool isRegister() const { return true; }
};

class AsmArgIndirect : public AsmArgRegister
{
private:
	int offset;
public:
	AsmArgIndirect(AsmRegistersT r, int shift = 0): AsmArgRegister(r), offset(shift) {}
	string generate() const { return "dword ptr [" + regName() + " + " + to_string(offset) + "]"; }
	bool operator == (AsmArg* o) const;
};

class AsmArgMemory : public AsmArg
{
private:
	string varName;
public:
	AsmArgMemory(const string& name): varName(name) {}
	string generate() const { return varName; }
	bool operator == (AsmArg* o) const;
	bool isMemoryLocation() const { return true; }
};

class AsmArgLabel : public AsmArg
{
private:
	string name;
public:
	AsmArgLabel(const string& n): name(n) {}
	string generate() const { return name; }
	bool operator == (AsmArg* o) const;
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
	virtual bool changeStack() const { return false; }
	virtual bool operateWith(AsmArg* arg) const { return false; }
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
	virtual string generate() const;
	AsmCommandsT code() const { return opCode; }
};

class AsmCmd1 : public AsmCmd
{
private:
	AsmArg *arg;
public:
	AsmCmd1(AsmCommandsT opcode, AsmArg* a): AsmCmd(opcode), arg(a) {}
	string generate() const;
	AsmArg* argument() { return arg; }
	bool changeStack() const { return opCode == cmdPUSH || opCode == cmdPOP || opCode == cmdRET || opCode == cmdCALL; }
	bool operateWith(AsmArg* a) const { return *arg == a; }
};

class AsmCmd2 : public AsmCmd
{
private:
	AsmArg *arg1, *arg2;
public:
	AsmCmd2(AsmCommandsT opcode, AsmArg* a1, AsmArg* a2): AsmCmd(opcode), arg1(a1), arg2(a2) {}
	string generate() const;
	AsmArg* firstArg() { return arg1; }
	AsmArg* secondArg() { return arg2; }
	bool changeStack() const;
	bool operateWith(AsmArg* a) const { return *arg1 == a || *arg2 == a; }
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
	bool changeStack() const { return true; }
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
	int size() const { return commands.size(); }
	void fflush(ofstream& out) const;
	void replace(int index, AsmCmd* cmd) { delete commands[index]; commands[index] = cmd; }
	void deleteRange(int l, int r);
	void insertBefore(AsmCmd* cmd, int idx);
	void move(int from, int to);
	AsmInstruction* operator [] (int idx) { return commands[idx]; }
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
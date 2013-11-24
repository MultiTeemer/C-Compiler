#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>

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
	cmdADD,
	cmdSUB,
	cmdINC,
	cmdDEC,
	
} AsmCommandsT;

typedef enum {
	EAX,
	EBX,
	ECX,
	EDX,
	EBP,
	ESP,
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
public:
	AsmArgIndirect(AsmRegistersT r): AsmArgRegister(r) {}
	string generate() const { return "[" + regName() + "]"; }
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

class AsmLabelEnd : public AsmLabel
{
public:
	AsmLabelEnd(AsmArgLabel* l): AsmLabel(l) {}
	string generate() const { return "end " + label->generate(); }
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

class AsmCode
{
private:
	vector<AsmInstruction*> commands;
public:
	static AsmArg* makeArg(const string& varName, bool isLabel = false);
	static AsmArg* makeArg(AsmRegistersT reg, bool indirect = false);
	static AsmArg* makeArg(int val);
	AsmCode(): commands(0) {}
	void fflush(ofstream& out) const;
	AsmCode& operator << (AsmCmd* command);
	AsmCode& add(AsmCommandsT cmd);
	AsmCode& add(AsmCommandsT cmd, AsmArg* arg);
	AsmCode& add(AsmCommandsT cmd, AsmArg* arg1, AsmArg* arg2);
	AsmCode& add(AsmArgLabel* label, bool end = false);
};

#endif
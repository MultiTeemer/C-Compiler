#include <iostream>
#include <fstream>
#include "CodeGenerator.h"
#include "Exceptions.h"

using namespace std;

string AsmArgRegister::regName() const
{
	switch (reg)
	{
	case EAX:
		return "eax";
	case EBX:
		return "ebx";
	case ECX:
		return "ecx";
	case EDX:
		return "edx";
	case EBP:
		return "ebp";
	case ESP:
		return "esp";
	default:
		throw exception("Illegal register value");
	}
}

string AsmCmd::cmdName() const
{
	switch (opCode)
	{
	case cmdMOV:
		return "mov";
	case cmdPUSH:
		return "push";
	case cmdPOP:
		return "pop";
	case cmdMUL:
		return "mul";
	case cmdIMUL:
		return "imul";
	case cmdDIV:
		return "div";
	case cmdADD:
		return "add";
	case cmdSUB:
		return "sub";
	case cmdINC:
		return "inc";
	case cmdDEC:
		return "dec";
	case cmdDB:
		return "db";
	case cmdDD:
		return "dd";
	case cmdDQ:
		return "dq";
	default:
		throw exception("Illegal command");
	}
}

string AsmCmd::generate() const 
{
	return cmdName();
}

string AsmCmd1::generate() const
{
	return cmdName() + " " + arg->generate();
}

string AsmCmd2::generate() const
{
	return opCode > cmdDQ 
		? cmdName() + " " + arg1->generate() + ", " + arg2->generate() 
		: arg1->generate() + " " + cmdName() + " " + arg2->generate();
}

AsmArg* AsmCode::makeArg(const string& varName, bool isLabel)
{
	if (isLabel)
		return new AsmArgLabel(varName);
	else
		return new AsmArgMemory(varName);
}

AsmArg* AsmCode::makeArg(AsmRegistersT reg, bool indirect)
{
	return indirect ? new AsmArgIndirect(reg) : new AsmArgRegister(reg);
}

AsmArg* AsmCode::makeArg(int val)
{
	return new AsmArgImmediate(val);
}

AsmCode& AsmCode::operator<<(AsmCmd* cmd)
{
	commands.push_back(cmd);
	return *this;
}

AsmCode& AsmCode::add(AsmCommandsT cmd)
{
	commands.push_back(new AsmCmd(cmd));
	return *this;
}

AsmCode& AsmCode::add(AsmCommandsT cmd, AsmArg* arg)
{
	commands.push_back(new AsmCmd1(cmd, arg));
	return *this;
}

AsmCode& AsmCode::add(AsmCommandsT cmd, AsmArg* arg1, AsmArg* arg2)
{
	commands.push_back(new AsmCmd2(cmd, arg1, arg2));
	return *this;
}

AsmCode& AsmCode::add(AsmArgLabel* label, bool end)
{
	commands.push_back(end ? new AsmLabelEnd(label) : new AsmLabel(label));
	return *this;
}

void AsmCode::fflush(ofstream& out) const
{
	for (int i = 0; i < commands.size(); i++)
		out << "\t" << commands[i]->generate() << endl;
}

void CodeGenerator::generate() const
{
	ofstream out(filename);
	if (!out)
		throw exception("Cannot open file");
	out << ".686\n"
		   ".model flat, stdcall\n"
		   "include c:\\masm32\\include\\msvcrt.inc\n"
		   "includelib c:\\masm32\\lib\\msvcrt.lib\n"
		   ".data\n";
	data.fflush(out);
	out << ".code\nmain:\n";
	code.fflush(out);
	out << "ret 0\nend main";
}
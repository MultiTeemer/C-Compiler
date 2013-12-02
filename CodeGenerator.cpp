#include <iostream>
#include <fstream>
#include "CodeGenerator.h"
#include "Exceptions.h"

using namespace std;

AsmArgMemory* makeArgMemory(const string& varName)
{
	return new AsmArgMemory(varName);
}

AsmArgRegister* makeArg(AsmRegistersT reg)
{
	return new AsmArgRegister(reg);
}

AsmArgImmediate* makeArg(int val)
{
	return new AsmArgImmediate(val);
}

AsmArgIndirect* makeIndirectArg(AsmRegistersT reg, int offset)
{
	return new AsmArgIndirect(reg, offset);
}

AsmArgLabel* makeLabel(const string& name)
{
	return new AsmArgLabel(name);
}

AsmArgString* makeString(const string& val)
{
	string str(val);
	for (int i = 0; i < str.length() - 2; i++)
		if (str.substr(i, 2) == "\\n")
		{
			string str1(str.substr(0, i)), str2(str.substr(i + 2));
			str = str1 + "\", 0dh, 0ah";
			if (str2.length() > 1)
				str += ", \"" + str2;
		}
	str += ", 0";
	return new AsmArgString(str);
}

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
	case CL:
		return "cl";
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
	case cmdIDIV:
		return "idiv";
	case cmdADD:
		return "add";
	case cmdSUB:
		return "sub";
	case cmdINC:
		return "inc";
	case cmdDEC:
		return "dec";
	case cmdRET:
		return "ret";
	case cmdDB:
		return "db";
	case cmdDD:
		return "dd";
	case cmdDQ:
		return "dq";
	case cmdINVOKE:
		return "invoke";
	case cmdXOR:
		return "xor";
	case cmdNEG:
		return "neg";
	case cmdCDQ:
		return "cdq";
	case cmdSHL:
		return "shl";
	case cmdSHR:
		return "shr";
	case cmdAND:
		return "and";
	case cmdOR:
		return "or";
	case cmdNOT:
		return "not";
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
	return cmdName() + 
		(opCode == cmdPUSH && (dynamic_cast<AsmArgIndirect*>(arg) || dynamic_cast<AsmArgImmediate*>(arg)) ? " dword ptr " : " ") 
		+ arg->generate();
}

string AsmCmd2::generate() const
{
	return opCode > cmdDQ 
		? cmdName() + " " + arg1->generate() + ", " + arg2->generate() 
		: arg1->generate() + " " + cmdName() + " " + arg2->generate();
}

string AsmIOCmd::generate() const
{
	string common = cmdName() + " crt_" + (mode == PRINTF ? "printf" : "scanf") + ", " + "addr " + format->generate();
	if (arg)
		return  common + ", " + arg->generate();
	else
		return common;
	
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

AsmCode& AsmCode::add(AsmCommandsT cmd, AsmRegistersT reg)
{
	commands.push_back(new AsmCmd1(cmd, new AsmArgRegister(reg)));
	return *this;
}

AsmCode& AsmCode::add(AsmCommandsT cmd, AsmRegistersT reg1, AsmRegistersT reg2)
{
	commands.push_back(new AsmCmd2(cmd, new AsmArgRegister(reg1), new AsmArgRegister(reg2)));
	return *this;
}

AsmCode& AsmCode::add(AsmCommandsT cmd, AsmRegistersT reg, int val)
{
	commands.push_back(new AsmCmd2(cmd, new AsmArgRegister(reg), new AsmArgImmediate(val)));
	return *this;
}

AsmCode& AsmCode::add(AsmArgLabel* label)
{
	commands.push_back(new AsmLabel(label));
	return *this;
}

AsmCode& AsmCode::add(OperationsT func, AsmArgMemory* format, AsmArg* arg)
{
	commands.push_back(new AsmIOCmd(func, format, arg));
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
	out << ".code\n";
	code.fflush(out);
	out << "start:\n\tcall f_main\n\tret 0\nend start";
}
#include <iostream>
#include <fstream>
#include "CodeGenerator.h"
#include "Exceptions.h"

using namespace std;

bool AsmArgRegister::operator==(AsmArg* o) const
{
	AsmArgRegister* tmp = dynamic_cast<AsmArgRegister*>(o);
	return tmp && tmp->reg == reg;
}

bool AsmArgIndirect::operator==(AsmArg* o) const
{
	AsmArgIndirect* tmp = dynamic_cast<AsmArgIndirect*>(o);
	return tmp && tmp->reg == reg && tmp->offset == offset;
}

bool AsmArgMemory::operator==(AsmArg* o) const
{
	AsmArgMemory* tmp = dynamic_cast<AsmArgMemory*>(o);
	return tmp && tmp->varName == varName;
}

bool AsmArgLabel::operator==(AsmArg* o) const
{
	AsmArgLabel* tmp = dynamic_cast<AsmArgLabel*>(o);
	return tmp && tmp->name == name;
}

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

AsmArgDup* makeArgDup(int count)
{
	return new AsmArgDup(count);
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

AsmArgFloat* makeFloat(float val)
{
	return new AsmArgFloat(val);
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
	case AL:
		return "al";
	case BL:
		return "bl";
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
	case cmdREAL8:
		return "real8";
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
	case cmdCALL:
		return "call";
	case cmdJMP:
		return "jmp";
	case cmdCMP:
		return "cmp";
	case cmdJE:
		return "je";
	case cmdJNE:
		return "jne";
	case cmdSETE:
		return "sete";
	case cmdSETNE:
		return "setne";
	case cmdSETG:
		return "setg";
	case cmdSETGE:
		return "setge";
	case cmdSETL:
		return "setl";
	case cmdSETLE:
		return "setle";
	case cmdFADDP:
		return "faddp";
	case cmdFDIVP:
		return "fdivp";
	case cmdFMULP:
		return "fmulp";
	case cmdFSUBP:
		return "fsubp";
	case cmdFLD:
		return "fld";
	case cmdFST:
		return "fst";
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
		(dynamic_cast<AsmArgImmediate*>(arg) && opCode != cmdRET ? " dword ptr " : " ") 
		+ arg->generate();
}

string AsmCmd2::generate() const
{
	return opCode > cmdDQ 
		? cmdName() + " " + arg1->generate() + ", " + arg2->generate() 
		: arg1->generate() + " " + cmdName() + " " + arg2->generate();
}

bool AsmCmd2::changeStack() const 
{
	return *arg1 == EBP || *arg1 == ESP || *arg2 == EBP || *arg2 == ESP;
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

void AsmCode::deleteRange(int l, int r) 
{
	for (int i = l; i <= r; i++)
		delete commands[i];
	commands.erase(commands.begin() + l, commands.begin() + r + 1);
}

void AsmCode::insertBefore(AsmCmd* cmd, int idx)
{
	commands.insert(commands.begin() + idx, cmd);
}

void AsmCode::move(int from, int to)
{
	AsmInstruction* tmp = commands[from];
	commands.erase(commands.begin() + from);
	commands.insert(commands.begin() + to, tmp);
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
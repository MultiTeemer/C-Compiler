#include "Optimizer.h"

using namespace std;

bool PushPop2MovOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd1* cmd1 = dynamic_cast<AsmCmd1*>(code[index]);
	AsmCmd1* cmd2 = dynamic_cast<AsmCmd1*>(code[index + 1]);
	if  (cmd1 && cmd1->code() == cmdPUSH && cmd2 && cmd2->code() == cmdPOP && *cmd1->argument() != cmd2->argument())
	{
		AsmCmd* optCmd = new AsmCmd2(cmdMOV, cmd2->argument(), cmd1->argument());
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);	
	} else 
		return false;
	return true;
}

bool PushPop2NilOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd1* cmd1 = dynamic_cast<AsmCmd1*>(code[index]);
	AsmCmd1* cmd2 = dynamic_cast<AsmCmd1*>(code[index + 1]);
	if  (cmd1 && cmd1->code() == cmdPUSH && cmd2 && cmd2->code() == cmdPOP 
		&& cmd1->argument()->isRegister() && *cmd1->argument() == cmd2->argument())
		code.deleteRange(index, index + 1);
	else 
		return false;
	return true;
}

bool MovChainOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	if (cmd1 && *cmd1->firstArg() == EAX && cmd2 && *cmd2->secondArg() == cmd1->firstArg()
		&& !cmd1->secondArg()->isMemoryLocation() && cmd1->code() == cmdMOV && cmd2->code() == cmdMOV)
	{
		AsmCmd* optCmd = new AsmCmd2(cmdMOV, cmd2->firstArg(), cmd1->secondArg());
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);
	} else 
		return false;
	return true;
}

Optimizer::Optimizer(): oneOpOpts(0), twoOpOpts(0), threeOpOpts(0)
{
	twoOpOpts.push_back(new PushPop2MovOptimization());
	twoOpOpts.push_back(new PushPop2NilOptimization());
	twoOpOpts.push_back(new MovChainOptimization());
}

void Optimizer::pushDownPopUp(AsmCode& code)
{
	for (int i = 0; i < code.size(); i++)
	{
		AsmCmd1* cmd = dynamic_cast<AsmCmd1*>(code[i]);
		if (cmd && cmd->code() == cmdPUSH)
		{			
			int j = i;
			while (j + 1 < code.size() && !code[j + 1]->changeStack() && !code[j + 1]->operateWith(cmd->argument()))
				j++;
			code.move(i, j);
		} else if (cmd && cmd->code() == cmdPOP) {
			int j = i;
			while (j - 1 > -1 && !code[j - 1]->changeStack() && !code[j - 1]->operateWith(cmd->argument()))
				j--;
			code.move(i, j);
		}
	}
}

void Optimizer::optimize(AsmCode& code)
{
	bool cycle = true;
	while (cycle)
	{
		pushDownPopUp(code);
		bool goToNextIteration = false;
		for (int i = 0; i < code.size(); i++)
			for (int j = 0; j < oneOpOpts.size(); j++)
				if (oneOpOpts[j]->optimize(code, i))
					goToNextIteration = true;				
		if (goToNextIteration)
			continue;
		for (int i = 0; i < code.size() - 1; i++)
			for (int j = 0; j < twoOpOpts.size(); j++)
				if (twoOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if (goToNextIteration)
			continue;
		for (int i = 0; i < code.size() - 2; i++)
			for (int j = 0; j < threeOpOpts.size(); j++)
				if (threeOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if (!goToNextIteration)
			break;
	}
}
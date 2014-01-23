#include "Optimizer.h"

using namespace std;

bool AddOrSubESPZeroOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd = dynamic_cast<AsmCmd2*>(code[index]);
	if (cmd && *cmd->firstArg() == ESP && *cmd->secondArg() == 0)
	{
		code.deleteRange(index, index);
	} else
		return false;
	return true;
}

bool PushPop2MovOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd1* cmd1 = dynamic_cast<AsmCmd1*>(code[index]);
	AsmCmd1* cmd2 = dynamic_cast<AsmCmd1*>(code[index + 1]);
	if  (
		cmd1 && *cmd1 == cmdPUSH && cmd2 && *cmd2 == cmdPOP 
		&& !(cmd1->argument()->isMemoryLocation() && cmd2->argument()->isMemoryLocation())
		)
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
	if  (cmd1 && *cmd1 == cmdPUSH && cmd2 && *cmd2 == cmdPOP 
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
	if (
		cmd1 && *cmd1 == cmdMOV && *cmd1->firstArg() == EAX 
		&& cmd2 && *cmd2 == cmdMOV && *cmd2->secondArg() == cmd1->firstArg()
		&& !(cmd1->secondArg()->isMemoryLocation() && cmd2->firstArg()->isMemoryLocation()) 
		)
	{
		AsmCmd* optCmd = new AsmCmd2(cmdMOV, cmd2->firstArg(), cmd1->secondArg());
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);
	} else 
		return false;
	return true;
}

bool AddZeroToEAX2NilOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	if (
		cmd1 && *cmd1 == cmdMOV && *cmd1->secondArg() == 0
		&& cmd2 && *cmd2 == cmdADD && *cmd1->firstArg() == cmd2->secondArg()
		)
		code.deleteRange(index, index + 1);
	else
		return false;
	return true;
}

bool Neg2MovOppositeOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd1* cmd2 = dynamic_cast<AsmCmd1*>(code[index + 1]);
	if (
		cmd1 && *cmd1 == cmdMOV && *cmd1->firstArg() == EAX
		&& cmd2 && *cmd2 == cmdNEG && *cmd2->argument() == EAX
		&& dynamic_cast<AsmArgImmediate*>(cmd1->secondArg())
		)
	{
		int val = dynamic_cast<AsmArgImmediate*>(cmd1->secondArg())->value;
		AsmCmd2* optCmd = new AsmCmd2(cmdMOV, makeArg(EAX), makeArg(-val));
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);
	} else
		return false;
	return true;
}

bool Jmp2NextLineOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd1* jmp = dynamic_cast<AsmCmd1*>(code[index]);
	AsmLabel* label = dynamic_cast<AsmLabel*>(code[index + 1]);
	if (jmp && label && *jmp->argument() == label->label)
		code.deleteRange(index, index);
	else
		return false;
	return true;
}

bool MovCycle2NilOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	if (
		cmd1 && *cmd1 == cmdMOV && cmd2 && *cmd2 == cmdMOV
		&& *cmd1->firstArg() == EBX && *cmd2->firstArg() == EAX
		&& *cmd1->firstArg() == cmd2->secondArg()
		&& *cmd1->secondArg() == cmd2->firstArg()
		)
		code.deleteRange(index, index + 1);
	else 
		return false;
	return true;
}

bool MovPush2PushOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd1* cmd2 = dynamic_cast<AsmCmd1*>(code[index + 1]);
	if (
		cmd1 && *cmd1 == cmdMOV && *cmd1->firstArg() == EAX
		&& cmd2 && *cmd2 == cmdPUSH && *cmd1->firstArg() == cmd2->argument()
		)
	{
		AsmCmd1* optCmd = new AsmCmd1(cmdPUSH, cmd1->secondArg());
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);
	} else 
		return false;
	return true;
}

bool RegRegCMP2RegIntCmpOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	if (
		cmd1 && *cmd1 == cmdMOV && cmd2 && *cmd2 == cmdCMP
		&& *cmd1->firstArg() == cmd2->secondArg()
		)
	{
		AsmCmd2* optCmd = new AsmCmd2(cmdCMP, cmd2->firstArg(), cmd1->secondArg());
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);
	} else
		return false;
	return true;
}

bool AddZero2MovOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	AsmCmd2* cmd3 = dynamic_cast<AsmCmd2*>(code[index + 2]);
	if (
		cmd1 && *cmd1 == cmdMOV && cmd2 && *cmd2 == cmdMOV
		&& cmd3 && *cmd3 == cmdADD && (*cmd1->secondArg() == 0 || *cmd2->secondArg() == 0)
		)
	{
		AsmCmd2* optCmd = new AsmCmd2(cmdMOV, cmd3->firstArg(), *cmd1->secondArg() == 0 ? cmd2->secondArg() : cmd1->secondArg());
		code.deleteRange(index, index + 2);
		code.insertBefore(optCmd, index);
	} else
		return false;
	return true;
}

bool CompactAdditionOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	AsmCmd2* cmd3 = dynamic_cast<AsmCmd2*>(code[index + 2]);
	if (
		cmd1 && *cmd1 == cmdMOV && cmd2 && *cmd2 == cmdMOV
		&& *cmd1->firstArg() == EBX && *cmd1->secondArg() == cmd2->firstArg()
		&& cmd3 && *cmd3 == cmdADD && *cmd3->firstArg() == cmd2->firstArg()
		&& *cmd3->secondArg() == cmd1->firstArg()
		)
	{
		AsmCmd2* optCmd = new AsmCmd2(cmdMOV, makeArg(EBX), cmd2->secondArg());
		code.deleteRange(index, index + 1);
		code.insertBefore(optCmd, index);
	} else
		return false;
	return true;
}


bool MultIntByInt2MovOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	AsmCmd2* cmd3 = dynamic_cast<AsmCmd2*>(code[index + 2]);
	if (
		cmd1 && *cmd1 == cmdMOV && cmd2 && *cmd2 == cmdMOV
		&& *cmd1->firstArg() == EAX && *cmd2->firstArg() == EBX
		&& cmd1->secondArg()->isImmediate() && cmd2->secondArg()->isImmediate()
		&& *cmd3 == cmdIMUL
		)
	{
		int val1 = dynamic_cast<AsmArgImmediate*>(cmd1->secondArg())->value,
			val2 = dynamic_cast<AsmArgImmediate*>(cmd2->secondArg())->value;
		AsmCmd2* optCmd = new AsmCmd2(cmdMOV, cmd3->firstArg(), makeArg(val1 * val2));
		code.deleteRange(index, index + 2);
		code.insertBefore(optCmd, index);
	} else 
		return false;
	return true;
}

bool Mov2MemoryDirectlyOptimization::optimize(AsmCode& code, int index) const
{
	AsmCmd2* cmd1 = dynamic_cast<AsmCmd2*>(code[index]);
	AsmCmd2* cmd2 = dynamic_cast<AsmCmd2*>(code[index + 1]);
	AsmCmd2* cmd3 = dynamic_cast<AsmCmd2*>(code[index + 2]);
	AsmCmd2* cmd4 = dynamic_cast<AsmCmd2*>(code[index + 3]);
	if (
		cmd1 && *cmd1->firstArg() == EAX && cmd1->secondArg()->isOffset()
		&& cmd2 && *cmd2->firstArg() == EBX && cmd2->secondArg()->isImmediate()
		&& cmd3 && cmd3->firstArg()->isMemoryLocation() && *cmd3->secondArg() == EBX
		&& cmd4 && *cmd4 == cmdMOV
		)
	{
		cmd1->secondArg()->clearOffset();
		AsmCmd2* optCmd1 = new AsmCmd2(cmdMOV, cmd1->secondArg(), cmd2->secondArg());
		AsmCmd2* optCmd2 = new AsmCmd2(cmdMOV, cmd4->firstArg(), cmd2->secondArg());
		code.deleteRange(index, index + 3);
		code.insertBefore(optCmd2, index);
		code.insertBefore(optCmd1, index);
	} else
		return false;
	return true;
}

Optimizer::Optimizer(): oneOpOpts(0), twoOpOpts(0), threeOpOpts(0), fourOpOpts(0)
{
	oneOpOpts.push_back(new AddOrSubESPZeroOptimization());
	
	twoOpOpts.push_back(new PushPop2NilOptimization());
	twoOpOpts.push_back(new PushPop2MovOptimization());
	twoOpOpts.push_back(new MovChainOptimization());
	twoOpOpts.push_back(new AddZeroToEAX2NilOptimization());
	twoOpOpts.push_back(new Neg2MovOppositeOptimization());
	twoOpOpts.push_back(new Jmp2NextLineOptimization());	
	twoOpOpts.push_back(new MovPush2PushOptimization());
	twoOpOpts.push_back(new RegRegCMP2RegIntCmpOptimization());
	
	threeOpOpts.push_back(new AddZero2MovOptimization());
	threeOpOpts.push_back(new MultIntByInt2MovOptimization());
	threeOpOpts.push_back(new CompactAdditionOptimization());
	
	fourOpOpts.push_back(new Mov2MemoryDirectlyOptimization());
	
	postTwoOpOpts.push_back(new MovCycle2NilOptimization());
}

void Optimizer::pushDownPopUp(AsmCode& code)
{
	for (int i = 0; i < code.size(); i++)
	{
		AsmCmd1* cmd = dynamic_cast<AsmCmd1*>(code[i]);
		if (cmd && *cmd == cmdPUSH)
		{			
			int j = i;
			while (j + 1 < code.size() && !code[j + 1]->changeStack() && !code[j + 1]->operateWith(cmd->argument()))
				j++;
			code.move(i, j);
		} else if (cmd && *cmd == cmdPOP) {
			int j = i;
			while (j - 1 > -1 && !code[j - 1]->changeStack() && !code[j - 1]->operateWith(cmd->argument()))
				j--;
			code.move(i, j);
		}
	}
}

void Optimizer::deleteUselessMovs(AsmCode& code)
{
	for (int i = 0; i < code.size(); i++)
	{
		if (*code[i] != cmdMOV || !code[i]->usesRegister(EAX))
			continue;
		AsmCmd2* cmd = dynamic_cast<AsmCmd2*>(code[i]);
		if (cmd->secondArg()->usesRegister(EAX))
			continue;
		if (*cmd->firstArg() != EAX)
			continue;
		int idx = i + 1;
		bool deletingNedeed = true;
		while (idx < code.size() && deletingNedeed)
		{
			if (code[idx]->usesRegister(EAX))
				if (*code[idx] != cmdMOV)
					deletingNedeed = false;
				else {
					AsmCmd2* tmp = dynamic_cast<AsmCmd2*>(code[idx]);
					if (tmp->secondArg()->usesRegister(EAX)
						|| dynamic_cast<AsmArgIndirect*>(tmp->firstArg()))
						deletingNedeed = false;
					else
						break;
				}
			idx++;
		}
		if (deletingNedeed)
			code.deleteRange(i, i);
	}
}

void Optimizer::deleteUselessLabels(AsmCode& code) 
{
	for (int i = 0; i < code.size(); i++)
	{
		AsmLabel* label = dynamic_cast<AsmLabel*>(code[i]);
		if (!label || *label == string("start"))
			continue;
		bool unused = true;
		for (int j = 0; j < code.size() && unused; j++)
		{
			if (j == i)
				continue;
			if (code[j]->isJump())
			{
				AsmArgLabel* dstn = dynamic_cast<AsmArgLabel*>(dynamic_cast<AsmCmd1*>(code[j])->argument());
				unused = *label->label != dstn;
			}
		}
		if (unused)
			code.deleteRange(i, i);
	}
}

void Optimizer::optimize(AsmCode& code)
{
	while (1)
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
		for (int i = 0; i < code.size() - 3; i++)
			for (int j = 0; j < fourOpOpts.size(); j++)
				if (fourOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if (!goToNextIteration)
			break;
	}
	deleteUselessMovs(code);
	deleteUselessLabels(code);
	while (1)
	{
		bool goToNextIteration = false;
		for (int i = 0; i < code.size() - 1; i++)
			for (int j = 0; j < postTwoOpOpts.size(); j++)
				if (postTwoOpOpts[j]->optimize(code, i))
					goToNextIteration = true;
		if (goToNextIteration)
			continue;
		if (!goToNextIteration)
			break;
	}
}
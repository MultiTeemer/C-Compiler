#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include "Commands.h"

using namespace std;

class Optimization
{
public:
	virtual bool optimize(AsmCode& code, int index) = 0;
};

template <class Type1>
class OneOperationOptimization : public Optimization
{
protected:
	Type1* cmd1;
public:
	OneOperationOptimization(): cmd1(0) {}
	bool prepare(AsmInstruction* ins) 
	{
		cmd1 = dynamic_cast<Type1*>(ins);
		return cmd1 != 0;
	}
};

template <class Type1, class Type2>
class TwoOperationOptimization : public OneOperationOptimization<Type1>
{
protected:
	Type2* cmd2;
public:
	TwoOperationOptimization(): cmd2(0) {}
	bool prepare(AsmInstruction* ins1, AsmInstruction* ins2)
	{
		cmd2 = dynamic_cast<Type2*>(ins2);
		return OneOperationOptimization<Type1>::prepare(ins1) && cmd2 != 0;
	}
};

template <class Type1, class Type2, class Type3>
class ThreeOperationOptimization : public TwoOperationOptimization<Type1, Type2>
{
protected:
	Type3* cmd3;
public:
	ThreeOperationOptimization(): cmd3(0) {}
	bool prepare(AsmInstruction* ins1, AsmInstruction* ins2, AsmInstruction* ins3)
	{
		cmd3 = dynamic_cast<Type3*>(ins3);
		return TwoOperationOptimization<Type1, Type2>::prepare(ins1, ins2) && cmd3 != 0;
	}
};

class FourOperationOptimization : public Optimization
{};

class AddOrSubESPZeroOptimization : public OneOperationOptimization<AsmCmd2>
{
public:
	bool optimize(AsmCode& code, int index);
};

typedef TwoOperationOptimization<AsmCmd1, AsmCmd1> C1C1Opt;
typedef TwoOperationOptimization<AsmCmd2, AsmCmd2> C2C2Opt;

class PushPop2MovOptimization : public C1C1Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class PushPop2NilOptimization : public C1C1Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class MovChainOptimization : public C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class AddZeroToEAX2NilOptimization : public C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class Neg2MovOppositeOptimization : public TwoOperationOptimization<AsmCmd2, AsmCmd1>
{
public:
	bool optimize(AsmCode& code, int index);
};

class Jmp2NextLineOptimization : public TwoOperationOptimization<AsmCmd1, AsmLabel>
{
public:
	bool optimize(AsmCode& code, int index);
};

class MovCycle2NilOptimization : public C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class MovPush2PushOptimization : public TwoOperationOptimization<AsmCmd2, AsmCmd1>
{
public:
	bool optimize(AsmCode& code, int index);
};

class RegRegCMP2RegIntCmpOptimization : public C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

typedef ThreeOperationOptimization<AsmCmd2, AsmCmd2, AsmCmd2> C2C2C2Opt;

class MultIntByInt2MovOptimization : public C2C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class AddZero2MovOptimization : public C2C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class CompactAdditionOptimization : public C2C2C2Opt
{
public:
	bool optimize(AsmCode& code, int index);
};

class Mov2MemoryDirectlyOptimization : public FourOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index);
};

class Optimizer
{
private:
	vector<Optimization*> oneOpOpts;
	vector<Optimization*> twoOpOpts;
	vector<Optimization*> threeOpOpts;
	vector<Optimization*> fourOpOpts;
	vector<Optimization*> postTwoOpOpts;
	void pushDownPopUp(AsmCode& code);
	void deleteUselessMovs(AsmCode& code);
	void deleteUselessLabels(AsmCode& code);
public:
	Optimizer();
	void optimize(AsmCode& code);
};


#endif
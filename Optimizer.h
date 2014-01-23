#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include "Commands.h"

using namespace std;

class Optimization
{
public:
	virtual bool optimize(AsmCode& code, int index) const = 0;
};

class OneOperationOptimization : public Optimization
{};

class TwoOperationOptimization : public Optimization
{};

class ThreeOperationOptimization : public Optimization
{};

class FourOperationOptimization : public Optimization
{};

class AddOrSubESPZeroOptimization : public OneOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class PushPop2MovOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class PushPop2NilOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class MovChainOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class AddZeroToEAX2NilOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class Neg2MovOppositeOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class Jmp2NextLineOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class MovCycle2NilOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class MovPush2PushOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class RegRegCMP2RegIntCmpOptimization : public TwoOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class MultIntByInt2MovOptimization : public ThreeOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class AddZero2MovOptimization : public ThreeOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class CompactAdditionOptimization : public ThreeOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class Mov2MemoryDirectlyOptimization : public FourOperationOptimization
{
public:
	bool optimize(AsmCode& code, int index) const;
};

class Optimizer
{
private:
	vector<OneOperationOptimization*> oneOpOpts;
	vector<TwoOperationOptimization*> twoOpOpts;
	vector<ThreeOperationOptimization*> threeOpOpts;
	vector<FourOperationOptimization*> fourOpOpts;
	vector<TwoOperationOptimization*> postTwoOpOpts;
	void pushDownPopUp(AsmCode& code);
	void deleteUselessMovs(AsmCode& code);
	void deleteUselessLabels(AsmCode& code);
public:
	Optimizer();
	void optimize(AsmCode& code);
};


#endif
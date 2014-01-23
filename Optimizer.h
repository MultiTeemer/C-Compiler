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
/*
template <typename Op1, typename Op2>
class Opt: public Optimization {
	virtual bool optimize1(Op1 op1, Op2 op2, AsmCode& code, int index) {
	}
	bool optimize(AsmCode& code, int index) const {
		//if ()
		return optimize1(dynamic_cast<Op1>(code[index]), dynamic_cast<Op1>(code[index]), code, index);
	}
};
*/
class OneOperationOptimization : public Optimization
{};

class TwoOperationOptimization : public Optimization
{};

class ThreeOperationOptimization : public Optimization
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

class Optimizer
{
private:
	vector<OneOperationOptimization*> oneOpOpts;
	vector<TwoOperationOptimization*> twoOpOpts;
	vector<ThreeOperationOptimization*> threeOpOpts;
	void pushDownPopUp(AsmCode& code);
public:
	Optimizer();
	void optimize(AsmCode& code);
};


#endif
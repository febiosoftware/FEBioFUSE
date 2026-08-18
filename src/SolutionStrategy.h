#pragma once
#include <FECore/FECoreClass.h>

struct FuseModel;

class FESolutionStrategy : public FECoreClass
{
public:
	FESolutionStrategy(FEModel* fem = nullptr);

	virtual bool RunCoupling(FuseModel& mdl) = 0;

	FECORE_BASE_CLASS(FESolutionStrategy)
};

class FETimeDecoupledStrategy : public FESolutionStrategy
{
public:
	FETimeDecoupledStrategy(FEModel* fem = nullptr);

	bool Init() override;

	bool RunCoupling(FuseModel& mdl) override;

private:
	int exchange_interval = 0;
	double exchange_time_interval = 0.0;

	int steps = 0;
	int exchanges = 0;

	DECLARE_FECORE_CLASS();
};

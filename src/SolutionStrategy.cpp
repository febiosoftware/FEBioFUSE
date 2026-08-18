#include "SolutionStrategy.h"
#include <FECore/log.h>
#include <FECore/FEModel.h>
#include "Model.h"
#include "FuseModel.h"
#include "DataExchange.h"
#include "DataExchangeList.h"

FESolutionStrategy::FESolutionStrategy(FEModel* fem) : FECoreClass(fem)
{

}

BEGIN_FECORE_CLASS(FETimeDecoupledStrategy, FESolutionStrategy)
	ADD_PARAMETER(exchange_interval, FE_RANGE_GREATER_OR_EQUAL(0), "exchange_interval");
	ADD_PARAMETER(exchange_time_interval, FE_RANGE_GREATER_OR_EQUAL(0), "exchange_time_interval");
END_FECORE_CLASS();

FETimeDecoupledStrategy::FETimeDecoupledStrategy(FEModel* fem) : FESolutionStrategy(fem)
{

}

bool FETimeDecoupledStrategy::Init()
{
	if (!FESolutionStrategy::Init()) return false;

	if (exchange_interval == 0 && exchange_time_interval == 0.0)
	{
		feLogError("Both exchange_interval and exchange_time_interval are zero. At least one must be non-zero.");
		return false;
	}
	return true;
}

bool FETimeDecoupledStrategy::RunCoupling(FuseModel& fuse)
{
	FETimeInfo& ti = GetFEModel()->GetTime();

	bool doExchange = false;

	if (exchange_interval > 0)
		doExchange = (steps % exchange_interval) == 0;

	if (!doExchange && exchange_time_interval > 0.0)
	{
		if (ti.currentTime - exchanges * exchange_time_interval >= -1e-6)
			doExchange = true;
	}

	if (doExchange)
	{
		feLog("\nTransferring data from primary to secondary.\n");
		if (!fuse.exchanges.TransferData(DataExchange::Type::PRIMARY_TO_SECONDARY))
			return false;

		// solve the secondary models
		feLog("Running secondary models:\n");
		for (SecondaryModel& mdl : fuse.models)
		{
			feLog("\tRunning model %s ...", mdl.name.c_str());
			bool b = mdl.RunModel(ti.currentTime);
			feLog(" %s\n", (b ? "SUCCESS" : "FAILED"));
			if (!b) return false;
		}

		feLog("\nTransferring data from secondary to primary.\n");
		if (!fuse.exchanges.TransferData(DataExchange::Type::SECONDARY_TO_PRIMARY))
			return false;

		exchanges++;
	}
	else {
		feLog("\nSkipping data exchange.\n");
	}
	steps++;

	return true;
}

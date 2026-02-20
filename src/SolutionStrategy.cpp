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
	ADD_PARAMETER(exchange_interval, FE_RANGE_GREATER(0), "exchange_interval");
END_FECORE_CLASS();

FETimeDecoupledStrategy::FETimeDecoupledStrategy(FEModel* fem) : FESolutionStrategy(fem)
{

}

bool FETimeDecoupledStrategy::RunCoupling(FuseModel& fuse)
{
	if ((exchanges % exchange_interval) == 0)
	{
		feLog("\nTransferring data from primary to secondary.\n");
		if (!fuse.exchanges.TransferData(DataExchange::Type::PRIMARY_TO_SECONDARY))
			return false;

		// solve the secondary models
		FETimeInfo& ti = GetFEModel()->GetTime();
		feLog("Running secondary models:\n");
		for (Model& mdl : fuse.models)
		{
			feLog("\tRunning model %s ...", mdl.name.c_str());
			bool b = mdl.RunModel(ti.currentTime);
			feLog(" %s\n", (b ? "SUCCESS" : "FAILED"));
			if (!b) return false;
		}

		feLog("\nTransferring data from secondary to primary.\n");
		if (!fuse.exchanges.TransferData(DataExchange::Type::SECONDARY_TO_PRIMARY))
			return false;
	}
	else {
		feLog("\nSkipping data exchange.\n");
	}
	exchanges++;

	return true;
}

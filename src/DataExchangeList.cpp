#include "DataExchangeList.h"
#include <FECore/FEModel.h>
#include <FECore/log.h>

BEGIN_FECORE_CLASS(DataExchangeList, FECoreClass)
	ADD_PROPERTY(exchanges, "exchange");
END_FECORE_CLASS();

bool DataExchangeList::TransferData(DataExchange::Type type)
{
	// transfer data
	for (DataExchange& dex : exchanges)
	{
		if (dex.type == type)
		{
			string srcName = dex.srcModel->GetName();
			string dstName = dex.dstModel->GetName();
			feLog("Mapping data from \"%s\" to \"%s\".\n", srcName.c_str(), dstName.c_str());
			dex.DoExchange();
		}
	}

	return true;
}

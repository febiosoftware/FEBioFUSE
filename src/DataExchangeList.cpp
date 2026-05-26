#include "DataExchangeList.h"
#include <FECore/FEModel.h>
#include <FECore/log.h>

BEGIN_FECORE_CLASS(DataExchangeList, FECoreClass)
	ADD_PROPERTY(exchanges, "exchange")->SetDefaultType("map-to-map");
END_FECORE_CLASS();

bool DataExchangeList::TransferData(DataExchange::Type type)
{
	// transfer data
	for (DataExchange* dex : exchanges)
	{
		if (dex->type == type)
		{
			dex->DoExchange();
		}
	}

	return true;
}

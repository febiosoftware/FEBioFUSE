#pragma once
#include <FECore/FECoreClass.h>
#include "DataExchange.h"
class DataExchangeList : public FECoreClass
{
public:
	DataExchangeList(FEModel* fem) : FECoreClass(fem) {}

	bool TransferData(DataExchange::Type type);

	std::vector<DataExchange>::iterator begin() { return exchanges.begin(); }
	std::vector<DataExchange>::iterator end() { return exchanges.end(); }
	bool empty() const { return exchanges.empty(); }

public:
	std::vector<DataExchange> exchanges;

	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(DataExchangeList);
};

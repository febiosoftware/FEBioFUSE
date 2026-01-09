#pragma once
#include <FECore/FECoreClass.h>
#include <FECore/FEDataValue.h>
#include <FECore/FEDataMap.h>
#include <FECore/FEFunction1D.h>
#include <memory>

class ModelList;

class DataExchange : public FECoreClass
{
public:
	enum class Type {
		INVALID,
		PRIMARY_TO_SECONDARY,
		SECONDARY_TO_PRIMARY
	};

public:
	DataExchange() {}

	bool InitExchange(ModelList& models);

	void DoExchange();

	std::string src;
	std::string dst;

	// TODO: I don't think this is currently deleted, so we may have a memory leak here. 
	FEFunction1D* filter = nullptr;

	FEModel* srcModel = nullptr;
	FEDataValue srcData;

	FEModel* dstModel = nullptr;
	FEDataMap* dstMap = nullptr;

	Type type = Type::INVALID;

	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(DataExchange);
};

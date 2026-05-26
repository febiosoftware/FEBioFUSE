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
	DataExchange(FEModel* fem) : FECoreClass(fem) {}

	virtual bool InitExchange(ModelList& models) = 0;

	virtual void DoExchange() = 0;

	std::string src;
	std::string dst;

	FEFunction1D* filter = nullptr;

	Type type = Type::INVALID;

	FECORE_BASE_CLASS(DataExchange);
};

class MapDataExchange : public DataExchange
{
public:
	MapDataExchange(FEModel* fem) : DataExchange(fem) {}

	bool InitExchange(ModelList& models) override;

	void DoExchange() override;

private:
	FEModel* srcModel = nullptr;
	FEDataValue srcData;

	FEModel* dstModel = nullptr;
	FEDataMap* dstMap = nullptr;

	DECLARE_FECORE_CLASS();
};

class ParamExchange : public DataExchange
{
public:
	ParamExchange(FEModel* fem) : DataExchange(fem) {}

	bool InitExchange(ModelList& models) override;

	void DoExchange() override;

private:
	FEModel* srcModel = nullptr;
	FEModel* dstModel = nullptr;

	DECLARE_FECORE_CLASS();
};

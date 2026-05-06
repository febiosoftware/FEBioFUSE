#pragma once
#include <FECore/FECoreClass.h>
#include "Model.h"

class PrimaryModel : public FECoreClass
{
public:
	PrimaryModel(FEModel* fem = nullptr) {}
	std::string name;
	FEModel* model = nullptr;
	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(PrimaryModelRef);
};

class ModelList : public FECoreClass
{
public:
	ModelList(FEModel* fem = nullptr) {}

	FEModel* GetModel(const std::string& name);

	void SetPrimaryModel(FEModel* fem) { primaryModel.model = fem; }
	FEModel* GetPrimaryModel() { return primaryModel.model; }

public:
	std::vector<SecondaryModel>::iterator begin() { return models.begin(); }
	std::vector<SecondaryModel>::iterator end() { return models.end(); }
	bool empty() const { return models.empty(); }

public:
	PrimaryModel primaryModel;
	std::vector<SecondaryModel> models;
	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(ModelList);
};


#pragma once
#include <FECore/FECoreClass.h>
#include "Model.h"

class PrimaryModel : public FECoreClass
{
public:
	std::string name;
	FEModel* model = nullptr;
	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(PrimaryModelRef);
};

class ModelList : public FECoreClass
{
public:
	FEModel* GetModel(const std::string& name);

	void SetPrimaryModel(FEModel* fem) { primaryModel.model = fem; }
	FEModel* GetPrimaryModel() { return primaryModel.model; }

public:
	std::vector<Model>::iterator begin() { return models.begin(); }
	std::vector<Model>::iterator end() { return models.end(); }
	bool empty() const { return models.empty(); }

public:
	PrimaryModel primaryModel;
	std::vector<Model> models;
	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(ModelList);
};


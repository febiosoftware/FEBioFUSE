#include "ModelList.h"
#include <FECore/FEModel.h>

BEGIN_FECORE_CLASS(PrimaryModel, FECoreClass)
	ADD_PARAMETER(name, "name")->SetFlags(FE_PARAM_ATTRIBUTE);
END_FECORE_CLASS();

BEGIN_FECORE_CLASS(ModelList, FECoreClass)
	ADD_PROPERTY(primaryModel, "primary_model");
	ADD_PROPERTY(models, "model");
END_FECORE_CLASS();

FEModel* ModelList::GetModel(const std::string& name)
{
	// check primary model first
	FEModel* fem = primaryModel.model;
	if (fem->GetName() == name) return fem;

	// check secondary models
	for (Model& m : models)
	{
		if (m.name == name)
		{
			assert(m.fem && (m.fem->GetName() == name));
			return m.fem.get();
		}
	}

	// something didn't go right
	return nullptr;
}

#include "Model.h"
#include <FECore/FEModel.h>
#include <FEBioXML/FEBioImport.h>
#include <iostream>

BEGIN_FECORE_CLASS(Model, FECoreClass)
	ADD_PARAMETER(name, "name")->SetFlags(FE_PARAM_ATTRIBUTE);
	ADD_PARAMETER(file, "file")->SetFlags(FE_PARAM_ATTRIBUTE);
END_FECORE_CLASS();

bool Model::InitModel()
{
	fem = make_shared<FEModel>();
	fem->SetName(name);

	// try to read the input file
	FEBioImport fim;
	if (fim.Load(*fem, file.c_str()) == false)
	{
		std::cerr << "Failed to load model file: " << file << std::endl;
		return false;
	}

	return true;
}

bool Model::RunModel(double currentTime)
{
	if (fem == nullptr) return false;
	fem->Reset();
	bool b = fem->Solve();

	if (b && plt)
	{
		plt->Write(currentTime);
	}

	return b;
}

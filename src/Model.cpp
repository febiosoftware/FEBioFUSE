#include "Model.h"
#include <FEBioXML/FEBioImport.h>
#include <FECore/FEAnalysis.h>
#include <FEBioLib/FEBioModelBuilder.h>
#include <iostream>

BEGIN_FECORE_CLASS(SecondaryModel, FECoreClass)
	ADD_PARAMETER(name, "name")->SetFlags(FE_PARAM_ATTRIBUTE);
	ADD_PARAMETER(file, "file")->SetFlags(FE_PARAM_ATTRIBUTE);
	ADD_PARAMETER(logOutput, "log_output");
END_FECORE_CLASS();

bool SecondaryModel::InitModel()
{
	fem = make_shared<FEBioModel>();
	fem->SetName(name);

	// try to read the input file
	FEBioImport fim;

	fim.SetModelBuilder(new FEBioModelBuilder(*fem));

	if (fim.Load(*fem, file.c_str()) == false)
	{
		std::cerr << "Failed to load model file: " << file << std::endl;
		return false;
	}

	// suppress all plot output for this model (we will handle it ourselves)
	for (int i = 0; i < fem->Steps(); ++i)
	{
		FEAnalysis* step = fem->GetStep(i);
		step->SetPlotLevel(FE_PLOT_NEVER);
	}

	if (!logOutput)
		fem->SetLogLevel(0);
	else
	{
		std::string logFile = name + ".log";
		fem->SetLogFilename(logFile.c_str());
	}

	return true;
}

bool SecondaryModel::RunModel(double currentTime)
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

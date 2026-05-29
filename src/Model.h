#pragma once
#include <FECore/FECoreClass.h>
#include <FEBioPlot/FEBioPlotFile.h>
#include <FEBioLib/FEBioModel.h>
#include <memory>

class SecondaryModel : public FECoreClass
{
public:
	SecondaryModel(FEModel* fem = nullptr) {}
	~SecondaryModel() {}
	bool InitModel();
	bool InitOutput();

	bool RunModel(double currentTime);

	std::string name;
	std::string file;

	bool logOutput = false;
	LogFileStream log;

	std::shared_ptr<FEBioModel> fem;
	std::shared_ptr<FEBioPlotFile> plt;


	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(SecondaryModel);
};

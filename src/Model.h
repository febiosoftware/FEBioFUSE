#pragma once
#include <FECore/FECoreClass.h>
#include <FEBioPlot/FEBioPlotFile.h>
#include <memory>

class SecondaryModel : public FECoreClass
{
public:
	SecondaryModel(FEModel* fem = nullptr) {}
	~SecondaryModel() {}
	bool InitModel();

	bool RunModel(double currentTime);

	std::string name;
	std::string file;
	std::shared_ptr<FEModel> fem;
	std::shared_ptr<FEBioPlotFile> plt;


	DECLARE_FECORE_CLASS();
	FECORE_BASE_CLASS(SecondaryModel);
};

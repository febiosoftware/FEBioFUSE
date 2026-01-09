#include "DataExchange.h"
#include "FECoupledSolver.h"
#include "ModelList.h"
#include <FECore/FEDomainMap.h>
#include <iostream>

BEGIN_FECORE_CLASS(DataExchange, FECoreClass)
	ADD_PARAMETER(src, "src")->SetFlags(FE_PARAM_ATTRIBUTE);
	ADD_PARAMETER(dst, "dst")->SetFlags(FE_PARAM_ATTRIBUTE);

	ADD_PROPERTY(filter, "filter");
END_FECORE_CLASS();

FEDataMap* GetDataMap(FEModel* fem, const std::string& name)
{
	if (name.empty()) return nullptr;
	ParamString ps(name.c_str());
	if (ps.string() != fem->GetName()) return nullptr;
	ps = ps.next();
	if (ps.string() != "mesh_data") return nullptr;

	string mapName = ps.IDString();
	if (mapName.empty()) return nullptr;

	FEMesh& mesh = fem->GetMesh();
	int N = mesh.DataMaps();
	for (int i = 0; i < N; ++i)
	{
		FEDataMap* dataMap = mesh.GetDataMap(i);
		if (dataMap->GetName() == mapName)
			return dataMap;
	}

	return nullptr;
}

bool DataExchange::InitExchange(ModelList& models)
{
	ParamString src(src.c_str());
	srcModel = models.GetModel(src.string()); assert(srcModel);
	if (srcModel == nullptr) return false;

	string moduleName = srcModel->GetModuleName();
	FECoreKernel& fecore = FECoreKernel::GetInstance();
	fecore.SetActiveModule(moduleName.c_str());

	srcData = srcModel->GetDataValue(src);
	if (!srcData.IsValid()) return false;

	ParamString psdst(dst.c_str());
	dstModel = models.GetModel(psdst.string()); assert(dstModel);
	if (dstModel == nullptr) return false;

	moduleName = dstModel->GetModuleName();
	fecore.SetActiveModule(moduleName.c_str());

	dstMap = GetDataMap(dstModel, dst);
	if (dstMap == nullptr) return false;

	if (srcModel == dstModel)
	{
		std::cerr << "Error: source and destination models cannot be the same.\n";
		return false;
	}

	FEModel* primaryModel = models.GetPrimaryModel();

	if (srcModel == primaryModel) type = DataExchange::Type::PRIMARY_TO_SECONDARY;
	else if (dstModel == primaryModel) type = DataExchange::Type::SECONDARY_TO_PRIMARY;

	if (type == DataExchange::Type::INVALID)
	{
		std::cerr << "Error: Invalid exchange type.\n";
		return false;
	}

	return true;
}

void DataExchange::DoExchange()
{
	// set destination map
	FEDomainMap* map = dynamic_cast<FEDomainMap*>(dstMap);
	const FEElementSet* eset = map->GetElementSet();
	std::vector<double> val;
	srcData.GetValues(eset, val);
	for (size_t i = 0; i < val.size(); ++i)
	{
		double v = val[i];
		if (filter) v = filter->value(v);
		map->setValue((int)i, v);
	}
}

#pragma once
#include "ModelList.h"
#include "DataExchangeList.h"

class FEModel;

struct FuseModel
{
	FuseModel(FEModel* fem) : exchanges(fem) {}

	ModelList models;
	DataExchangeList exchanges;
};

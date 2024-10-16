#include "pch.h"
#include "CMeshDataUI.h"

CMeshDataUI::CMeshDataUI()
	: CAssetUI(ASSET_TYPE::MESH_DATA)
{
}

CMeshDataUI::~CMeshDataUI()
{
}


void CMeshDataUI::Update()
{
	Title();
}

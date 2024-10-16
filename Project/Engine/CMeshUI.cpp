#include "pch.h"
#include "CMeshUI.h"

CMeshUI::CMeshUI()
	: CAssetUI(ASSET_TYPE::MESH)
{
}

CMeshUI::~CMeshUI()
{
}

void CMeshUI::Update()
{
	Title();
}

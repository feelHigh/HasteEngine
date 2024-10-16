#include "pch.h"
#include "CPrefabUI.h"

CPrefabUI::CPrefabUI()
	: CAssetUI(ASSET_TYPE::PREFAB)
{
}

CPrefabUI::~CPrefabUI()
{
}

void CPrefabUI::Update()
{
	Title();
}

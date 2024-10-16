#include "pch.h"
#include "CPrefabUI.h"

void CPrefabUI::Update()
{
	Title();

}

CPrefabUI::CPrefabUI()
	: CAssetUI(ASSET_TYPE::PREFAB)
{
}

CPrefabUI::~CPrefabUI()
{
}

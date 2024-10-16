#include "pch.h"
#include "CSoundUI.h"

CSoundUI::CSoundUI()
	: CAssetUI(ASSET_TYPE::SOUND)
{
}

CSoundUI::~CSoundUI()
{
}

void CSoundUI::Update()
{
	Title();
}

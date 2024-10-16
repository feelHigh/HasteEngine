#include "pch.h"
#include "CSpriteUI.h"

CSpriteUI::CSpriteUI()
	: CAssetUI(ASSET_TYPE::SPRITE)
{
}

CSpriteUI::~CSpriteUI()
{
}

void CSpriteUI::Update()
{
	Title();
}

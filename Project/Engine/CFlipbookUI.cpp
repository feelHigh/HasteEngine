#include "pch.h"
#include "CFlipbookUI.h"

CFlipbookUI::CFlipbookUI()
	: CAssetUI(ASSET_TYPE::FLIPBOOK)
{
}

CFlipbookUI::~CFlipbookUI()
{
}

void CFlipbookUI::Update()
{
	Title();
}

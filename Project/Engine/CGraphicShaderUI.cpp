#include "pch.h"
#include "CGraphicShaderUI.h"

CGraphicShaderUI::CGraphicShaderUI()
	: CAssetUI(ASSET_TYPE::GRAPHIC_SHADER)
{
}

CGraphicShaderUI::~CGraphicShaderUI()
{
}

void CGraphicShaderUI::Update()
{
	Title();
}

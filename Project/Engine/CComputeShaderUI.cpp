#include "pch.h"
#include "CComputeShaderUI.h"

CComputeShaderUI::CComputeShaderUI()
	: CAssetUI(ASSET_TYPE::COMPUTE_SHADER)
{
}

CComputeShaderUI::~CComputeShaderUI()
{
}

void CComputeShaderUI::Update()
{
	Title();
}

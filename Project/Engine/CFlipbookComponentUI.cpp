#include "pch.h"
#include "CFlipbookComponentUI.h"

CFlipbookComponentUI::CFlipbookComponentUI()
	: CComponentUI(COMPONENT_TYPE::FLIPBOOKCOMPONENT)
{
}

CFlipbookComponentUI::~CFlipbookComponentUI()
{
}

void CFlipbookComponentUI::Update()
{
	Title();
}

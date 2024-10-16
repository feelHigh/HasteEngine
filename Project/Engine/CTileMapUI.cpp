#include "pch.h"
#include "CTileMapUI.h"

CTileMapUI::CTileMapUI()
	: CComponentUI(COMPONENT_TYPE::TILEMAP)
{
}

CTileMapUI::~CTileMapUI()
{
}

void CTileMapUI::Update()
{
	Title();
}

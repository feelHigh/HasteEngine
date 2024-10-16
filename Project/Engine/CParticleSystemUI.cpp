#include "pch.h"
#include "CParticleSystemUI.h"

CParticleSystemUI::CParticleSystemUI()
	: CComponentUI(COMPONENT_TYPE::PARTICLESYSTEM)
{
}

CParticleSystemUI::~CParticleSystemUI()
{
}

void CParticleSystemUI::Update()
{
	Title();
}

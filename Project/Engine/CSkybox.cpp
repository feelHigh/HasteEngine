#include "pch.h"
#include "CSkybox.h"

// Manager Headers
#include "CAssetMgr.h"

// Component Headers
#include "CTransform.h"

CSkybox::CSkybox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SPHERE)
{
	SetSkyboxType(m_Type);
}

CSkybox::~CSkybox()
{
}

void CSkybox::SetSkyboxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (SKYBOX_TYPE::SPHERE == m_Type)
		SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"SphereMesh"));
	else if (SKYBOX_TYPE::CUBE == m_Type)
		SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"CubeMesh"));

	// Mesh �� ����Ǹ� ������ �ٽ� �����ؾ� �Ѵ�.
	SetMaterial(CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}

void CSkybox::FinalTick()
{
	// 1. ���� ī�޶� �� �ִ� �þ߰Ÿ���, Transform �� Scale �� �����Ѵ�.
	//  - Play -> Main Cam
	//  - Pause, Stop -> Editor Cam

	// 2. SkyBox ��ġ�� ���� ī�޶� ��ġ�� �ű��.

}

void CSkybox::Render()
{
	Transform()->Binding();

	GetMaterial()->SetScalarParametereter(INT_0, (int)m_Type);

	if (m_Type == SPHERE)
	{
		if (!m_SkyboxTex->IsCubeMap())
			GetMaterial()->SetTextureParameter(TEX_0, m_SkyboxTex);
		else
			GetMaterial()->SetTextureParameter(TEX_0, nullptr);
	}

	else if (m_Type == CUBE)
	{
		if (m_SkyboxTex->IsCubeMap())
			GetMaterial()->SetTextureParameter(TEXCUBE_0, m_SkyboxTex);
		else
			GetMaterial()->SetTextureParameter(TEXCUBE_0, nullptr);
	}

	GetMaterial()->Binding();

	GetMesh()->Render();
}

void CSkybox::SaveToFile(FILE* _File)
{
}

void CSkybox::LoadFromFile(FILE* _File)
{
}

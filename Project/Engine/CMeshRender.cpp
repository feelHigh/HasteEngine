#include "pch.h"
#include "CMeshRender.h"

// Asset Headers
#include "CSprite.h"

// Component Headers
#include "CTransform.h"
#include "CFlipbookComponent.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	if (!GetMesh() || !GetMaterial() || !GetMaterial()->GetShader())
		return;

	// FlipBookComponent �� ������ ���� ������� Sprite ������ Binding �ϰ� �Ѵ�.
	if (FlipbookComponent())
		FlipbookComponent()->Binding();
	else
		CFlipbookComponent::Clear();

	// ��ġ, ũ��, ȸ�� �������� ���ε�
	Transform()->Binding();

	// ���� ���ε�(���� ���, ���̴� ���)
	GetMaterial()->Binding();

	// ���ؽ�����, �ε������� ���ε� �� ������ ȣ��
	GetMesh()->Render();
}

void CMeshRender::SaveToFile(FILE* _File)
{
	SaveDataToFile(_File);
}

void CMeshRender::LoadFromFile(FILE* _File)
{
	LoadDataFromFile(_File);
}

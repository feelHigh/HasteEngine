#include "pch.h"
#include "CTransform.h"

// Device Headers
#include "CDevice.h"
#include "CConstantBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeDir{}
	, m_WorldDir{}
	, m_IndependentScale(false)
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalTick()
{
	// ������Ʈ�� ������� ���
	// ũ�����
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	// �̵� ���
	Matrix matTranslation = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// ȸ�� ���
	Matrix matRot = XMMatrixRotationX(m_RelativeRot.x)
					* XMMatrixRotationY(m_RelativeRot.y)
					* XMMatrixRotationZ(m_RelativeRot.z);

	m_matWorld = matScale * matRot * matTranslation;

	// ���⺤�� ���
	static Vec3 vDefaultAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_RelativeDir[i] = XMVector3TransformNormal(vDefaultAxis[i], matRot);
		m_RelativeDir[i].Normalize();
	}

	// �θ� ������Ʈ�� �ִ��� Ȯ��
	if (GetOwner()->GetParent())
	{
		// �θ��� ��������� ���ؼ� ���� ��������� �����
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_IndependentScale)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			Matrix matParentScale = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			Matrix matParentScaceInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * matParentScaceInv * matParentWorldMat;
		}
		else
		{
			m_matWorld *= matParentWorldMat;
		}

		// ���� ������� ������Ʈ�� ���⺤�͸� ����
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(vDefaultAxis[i], m_matWorld);
		}
	}

	// �θ� ������, RelativeDir �� �� WorldDir
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = m_RelativeDir[i];
		}
	}
}

void CTransform::Binding()
{
	g_Trans.matWorld = m_matWorld;
	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	CConstantBuffer* pTransformCB = CDevice::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_Trans);
	pTransformCB->Binding();
}


Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = Vec3(1.f, 1.f, 1.f);

	CGameObject* pObject = GetOwner();

	while (pObject)
	{
		vWorldScale *= pObject->Transform()->GetRelativeScale();

		if (pObject->Transform()->m_IndependentScale)
			break;

		pObject = pObject->GetParent();
	}

	return vWorldScale;
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRot, sizeof(Vec3), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}
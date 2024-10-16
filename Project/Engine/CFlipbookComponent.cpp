#include "pch.h"
#include "CFlipbookComponent.h"

// Device Headers
#include "CDevice.h"
#include "CConstantBuffer.h"

// Manager Headers
#include "CTimeMgr.h"
#include "CAssetMgr.h"

// Asset Headers
#include "CFlipbook.h"

CFlipbookComponent::CFlipbookComponent()
	: CComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT)
	, m_CurFlipbook(nullptr)
	, m_CurFrmIdx(0)
{
}

CFlipbookComponent::CFlipbookComponent(CFlipbookComponent& _Origin)
	: CComponent(_Origin)
	, m_vecFlipbook(_Origin.m_vecFlipbook)
	, m_CurFlipbook(_Origin.m_CurFlipbook)
	, m_CurFrmIdx(0)
	, m_FPS(_Origin.m_FPS)
	, m_AccTime(0.f)
	, m_Repeat(_Origin.m_Repeat)
	, m_Finish(false)
{
	if (nullptr != m_CurFlipbook)
	{
		int FlipbookIdx = 0;
		for (; FlipbookIdx < (int)m_vecFlipbook.size(); ++FlipbookIdx)
		{
			if (m_CurFlipbook == m_vecFlipbook[FlipbookIdx])
				break;
		}

		Play(FlipbookIdx, m_FPS, m_Repeat);
	}
}

CFlipbookComponent::~CFlipbookComponent()
{
}

void CFlipbookComponent::FinalTick()
{
	if (m_Finish)
	{
		if (false == m_Repeat)
			return;

		Reset();
	}

	if (nullptr != m_CurFlipbook)
	{
		float MaxTime = 1.f / m_FPS;

		m_AccTime += DT;

		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurFrmIdx;

			if (m_CurFlipbook->GetMaxFrameCount() <= m_CurFrmIdx)
			{
				--m_CurFrmIdx;
				m_Finish = true;
			}
		}

		m_CurFrmSprite = m_CurFlipbook->GetSprite(m_CurFrmIdx);
	}
}

void CFlipbookComponent::AddFlipbook(int _Idx, Ptr<CFlipbook> _Flipbook)
{
	if (m_vecFlipbook.size() <= _Idx)
	{
		m_vecFlipbook.resize(_Idx + 1);
	}

	m_vecFlipbook[_Idx] = _Flipbook;
}

Ptr<CFlipbook> CFlipbookComponent::FindFlipbook(const wstring& _Key)
{
	for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
	{
		if (m_vecFlipbook[i]->GetKey() == _Key)
			return m_vecFlipbook[i];
	}

	return nullptr;
}

void CFlipbookComponent::Play(int _FliBookIdx, float _FPS, bool _Repeat)
{
	m_CurFlipbook = m_vecFlipbook[_FliBookIdx];

	if (nullptr == m_CurFlipbook)
	{
		return;
	}

	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_FPS = _FPS;
	m_Repeat = _Repeat;
}

void CFlipbookComponent::Reset()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_Finish = false;
}

void CFlipbookComponent::Binding()
{
	if (nullptr != m_CurFrmSprite)
	{
		// ���� ǥ���ؾ��ϴ� Sprite �� ������ Sprite ���� ������۸� ���ؼ� GPU �� ����
		tSpriteInfo tInfo = {};

		tInfo.LeftTopUV = m_CurFrmSprite->GetLeftTopUV();
		tInfo.SliceUV = m_CurFrmSprite->GetSliceUV();
		tInfo.BackGroundUV = m_CurFrmSprite->GetBackgroundUV();
		tInfo.OffsetUV = m_CurFrmSprite->GetOffsetUV();
		tInfo.UseFlipbook = 1;

		static CConstantBuffer* CB = CDevice::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::SPRITE);

		CB->SetData(&tInfo);
		CB->Binding();

		// Flipbook Sprite ��Ʋ�� �ؽ��� ���� �������͹�ȣ t10 �� ���ε�
		Ptr<CTexture> pAtlas = m_CurFrmSprite->GetAtlasTexture();
		pAtlas->Binding(10);
	}
	else
	{
		Clear();
	}
}

void CFlipbookComponent::Clear()
{
	tSpriteInfo tInfo = {};
	static CConstantBuffer* CB = CDevice::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::SPRITE);
	CB->SetData(&tInfo);
	CB->Binding();
}

void CFlipbookComponent::SaveToFile(FILE* _File)
{
	// Flipbook ���� ��� ����
	size_t FlipbookCount = m_vecFlipbook.size();
	fwrite(&FlipbookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
	{
		SaveAssetRef(m_vecFlipbook[i], _File);
	}

	// ���� ������� Flipbook ���� ����
	SaveAssetRef(m_CurFlipbook, _File);

	// ���� ������� Flipbook ������ ������ Sprite
	SaveAssetRef(m_CurFrmSprite, _File);

	// ���� ������� Flipbook ������ ������ Sprite �� ���° �ε�������
	fwrite(&m_CurFrmIdx, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);
	fwrite(&m_AccTime, sizeof(float), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
}

void CFlipbookComponent::LoadFromFile(FILE* _File)
{
	// Flipbook ���� ��� �ҷ�����
	size_t FlipbookCount = 0;
	fread(&FlipbookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		Ptr<CFlipbook> pFlipbook;
		LoadAssetRef(pFlipbook, _File);
		m_vecFlipbook.push_back(pFlipbook);
	}

	// ���� ������� Flipbook ���� �ε�
	LoadAssetRef(m_CurFlipbook, _File);

	// ���� ������� Flipbook ������ ������ Sprite
	LoadAssetRef(m_CurFrmSprite, _File);

	// ���� ������� Flipbook ������ ������ Sprite �� ���° �ε�������
	fread(&m_CurFrmIdx, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
	fread(&m_AccTime, sizeof(float), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
}

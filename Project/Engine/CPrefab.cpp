#include "pch.h"
#include "CPrefab.h"

// GameObject Headers
#include "CGameObject.h"

OBJECT_SAVE  CPrefab::g_ObjectSaveFunc = nullptr;
OBJECT_LOAD  CPrefab::g_ObjectLoadFunc = nullptr;

CPrefab::CPrefab(bool _IsEngine)
    : CAsset(ASSET_TYPE::PREFAB)
    , m_ProtoObject(nullptr)
{
    if (_IsEngine)
    {
        SetEngineAsset();
    }
}

CPrefab::~CPrefab()
{
    if (nullptr != m_ProtoObject)
        delete m_ProtoObject;
}

CGameObject* CPrefab::Instantiate()
{
    return m_ProtoObject->Clone();
}

int CPrefab::Save(const wstring& _FilePath)
{
    FILE* File = nullptr;
    _wfopen_s(&File, _FilePath.c_str(), L"wb");

    if (nullptr == File)
        return E_FAIL;

    g_ObjectSaveFunc(File, m_ProtoObject);

    fclose(File);

    return S_OK;
}

int CPrefab::Load(const wstring& _FilePath)
{
    FILE* File = nullptr;
    _wfopen_s(&File, _FilePath.c_str(), L"rb");

    if (nullptr == File)
        return E_FAIL;

    m_ProtoObject = g_ObjectLoadFunc(File);

    fclose(File);

    return S_OK;
}

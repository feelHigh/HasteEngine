#pragma once
#include "CEntity.h"

class CAsset :
    public CEntity
{
public:
    CLONE_DISABLE(CAsset);
    CAsset(ASSET_TYPE _Type);
    CAsset(const CAsset& _Other);
    ~CAsset();

    template<typename T>
    friend class Ptr;

    friend class CAssetMgr;

public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
    ASSET_TYPE GetAssetType() { return m_Type; }
    UINT GetRefCount() { return m_RefCount; }
    bool IsEngineAsset() { return m_Engine; }

private:
    void AddRef() { ++m_RefCount; }
    void Release()
    {
        --m_RefCount;
        if (m_RefCount <= 0)
        {
            delete this;
        }
    }

private:
    virtual int Load(const wstring& _FilePath) = 0;
    virtual int Save(const wstring& _RelativePath) = 0;

private:
    wstring             m_Key;
    wstring             m_RelativePath;
    const ASSET_TYPE    m_Type;
    int                 m_RefCount;

    bool                m_Engine;

protected:
    void SetEngineAsset() { m_Engine = true; }

protected:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _path) { m_RelativePath = _path; }

};

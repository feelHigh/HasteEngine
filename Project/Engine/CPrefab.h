#pragma once
#include "CAsset.h"

#include "CEngine.h"

class CPrefab :
    public CAsset
{
public:
    CPrefab(bool _IsEngine = false);
    ~CPrefab();

    friend class CEngine;

public:
    void SetProtoObject(CGameObject* _Object) { m_ProtoObject = _Object; }
    CGameObject* Instantiate();

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    static OBJECT_SAVE  g_ObjectSaveFunc;
    static OBJECT_LOAD  g_ObjectLoadFunc;

private:
    class CGameObject* m_ProtoObject;

};

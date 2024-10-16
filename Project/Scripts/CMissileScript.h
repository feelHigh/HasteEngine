#pragma once
#include <Engine/CScript.h>

class CMissileScript :
    public CScript
{
public:
    CLONE(CMissileScript);
    CMissileScript();
    ~CMissileScript();

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    float   m_Speed;

};

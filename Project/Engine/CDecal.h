#ifndef CDECAL_H
#define CDECAL_H

#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
public:
    CDecal();
    ~CDecal();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    void SetDecalTexture(Ptr<CTexture> _Tex) { m_DecalTex = _Tex; }

public:
    virtual void SaveToFile(FILE* _File) override;   // ���Ͽ� ������Ʈ�� ������ ����
    virtual void LoadFromFile(FILE* _File) override; // ���Ͽ� ������Ʈ�� ������ �ε�

private:
    Ptr<CTexture>   m_DecalTex;

};

#endif // CDECAL_H
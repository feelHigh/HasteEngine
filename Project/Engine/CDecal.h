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
    virtual void SaveToFile(FILE* _File) override;   // 파일에 컴포넌트의 정보를 저장
    virtual void LoadFromFile(FILE* _File) override; // 파일에 컴포넌트의 정보를 로드

private:
    Ptr<CTexture>   m_DecalTex;

};

#endif // CDECAL_H
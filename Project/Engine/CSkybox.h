#ifndef CSKYBOX_H
#define CSKYBOX_H

#include "CRenderComponent.h"

enum SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};

class CSkybox :
    public CRenderComponent
{
public:
    CLONE(CSkybox);
    CSkybox();
    CSkybox(const CSkybox& _Origin);
    ~CSkybox();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    void SetSkyboxType(SKYBOX_TYPE _Type);
    void SetSkyboxTexture(Ptr<CTexture> _Tex) { m_SkyboxTex = _Tex; }

public:
    virtual void SaveToFile(FILE* _File) override;   // ���Ͽ� ������Ʈ�� ������ ����
    virtual void LoadFromFile(FILE* _File) override; // ���Ͽ� ������Ʈ�� ������ �ε�

private:
    SKYBOX_TYPE     m_Type;
    Ptr<CTexture>   m_SkyboxTex;

};

#endif // CSKYBOX_H
#pragma once
#include "CAssetUI.h"

class CMaterialUI
    : public CAssetUI
{
public:
    CMaterialUI();
    ~CMaterialUI();

public:
    virtual void Update() override;

private:
    void SelectShader(DWORD_PTR _ListUI);
    void SaveMaterialToFile();

private:
    void ShaderParameter();
    void ChangeTexture(DWORD_PTR Param);

private:
    TEXTURE_PARAMETER   m_SelectTexParam;
     
};

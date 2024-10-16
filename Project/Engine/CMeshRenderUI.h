#pragma once
#include "CComponentUI.h"

class CMeshRenderUI :
    public CComponentUI
{
public:
    CMeshRenderUI();
    ~CMeshRenderUI();

public:
    void SelectMesh(DWORD_PTR _AssetName);
    void SelectMaterial(DWORD_PTR _AssetName);

private:
    virtual void Update() override;

};

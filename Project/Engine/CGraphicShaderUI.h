#pragma once
#include "CAssetUI.h"

class CGraphicShaderUI :
    public CAssetUI
{
public:
    CGraphicShaderUI();
    ~CGraphicShaderUI();

public:
    virtual void Update() override;

};

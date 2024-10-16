#pragma once
#include "CAssetUI.h"

class CComputeShaderUI :
    public CAssetUI
{
public:
    CComputeShaderUI();
    ~CComputeShaderUI();

public:
    virtual void Update() override;

};

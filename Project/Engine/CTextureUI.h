#pragma once
#include "CAssetUI.h"

class CTextureUI :
    public CAssetUI
{
public:
    CTextureUI();
    ~CTextureUI();

public:
    virtual void Update() override;

};

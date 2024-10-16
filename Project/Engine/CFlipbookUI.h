#pragma once
#include "CAssetUI.h"

class CFlipbookUI :
    public CAssetUI
{
public:
    CFlipbookUI();
    ~CFlipbookUI();

public:
    virtual void Update() override;

};

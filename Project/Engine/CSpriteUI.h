#pragma once
#include "CAssetUI.h"

class CSpriteUI :
    public CAssetUI
{
public:
    CSpriteUI();
    ~CSpriteUI();

public:
    virtual void Update() override;

};

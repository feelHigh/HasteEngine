#pragma once
#include "CAssetUI.h"

class CSoundUI :
    public CAssetUI
{
public:
    CSoundUI();
    ~CSoundUI();

public:
    virtual void Update() override;

};

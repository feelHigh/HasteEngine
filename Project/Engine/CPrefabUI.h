#pragma once
#include "CAssetUI.h"

class CPrefabUI :
    public CAssetUI
{
public:
    CPrefabUI();
    ~CPrefabUI();

public:
    virtual void Update() override;

};

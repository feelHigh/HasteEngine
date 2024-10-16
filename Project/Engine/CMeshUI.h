#pragma once
#include "CAssetUI.h"

class CMeshUI :
    public CAssetUI
{
public:
    CMeshUI();
    ~CMeshUI();

public:
    virtual void Update() override;

};

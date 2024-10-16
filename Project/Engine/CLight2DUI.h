#pragma once
#include "CComponentUI.h"

class CLight2DUI :
    public CComponentUI
{
public:
    CLight2DUI();
    ~CLight2DUI();

public:
    virtual void Update() override;

};

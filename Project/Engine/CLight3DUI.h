#pragma once
#include "CComponentUI.h"

class CLight3DUI :
    public CComponentUI
{
public:
    CLight3DUI();
    ~CLight3DUI();

public:
    virtual void Update() override;

};

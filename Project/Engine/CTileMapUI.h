#pragma once
#include "CComponentUI.h"

class CTileMapUI :
    public CComponentUI
{
public:
    CTileMapUI();
    ~CTileMapUI();

public:
    virtual void Update() override;

};

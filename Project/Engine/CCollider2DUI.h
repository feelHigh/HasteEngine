#pragma once
#include "CComponentUI.h"

class CCollider2DUI :
    public CComponentUI
{
public:
    CCollider2DUI();
    ~CCollider2DUI();

public:
    virtual void Update() override;

};

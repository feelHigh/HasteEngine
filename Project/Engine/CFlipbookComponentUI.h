#pragma once
#include "CComponentUI.h"

class CFlipbookComponentUI :
    public CComponentUI
{
public:
    CFlipbookComponentUI();
    ~CFlipbookComponentUI();

private:
    virtual void Update() override;

};

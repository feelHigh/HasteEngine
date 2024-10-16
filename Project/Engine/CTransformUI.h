#pragma once
#include "CComponentUI.h"

class CTransformUI :
    public CComponentUI
{
public:
    CTransformUI();
    ~CTransformUI();

public:
    virtual void Update() override;

};

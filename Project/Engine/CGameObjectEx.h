#pragma once
#include "CGameObject.h"

class CGameObjectEx :
    public CGameObject
{
public:
    CLONE(CGameObjectEx);

public:
    virtual void FinalTick() override;

};

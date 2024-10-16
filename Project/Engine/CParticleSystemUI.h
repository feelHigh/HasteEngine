#pragma once
#include "CComponentUI.h"

class CParticleSystemUI :
    public CComponentUI
{
public:
    CParticleSystemUI();
    ~CParticleSystemUI();

public:
    virtual void Update() override;

};

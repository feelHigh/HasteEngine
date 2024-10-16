#pragma once
#include "CComponentUI.h"

class CCameraUI :
    public CComponentUI
{
public:
    CCameraUI();
    ~CCameraUI();

private:
    virtual void Update() override;

private:
    void LayerCheck();
    void Projection();

private:
    bool    m_ShowLayerCheck;

};

#pragma once
#include "CComputeShader.h"

class CSetColorCS :
    public CComputeShader
{
public:
    CSetColorCS();
    ~CSetColorCS();

public:
    virtual int Binding() override;
    virtual void CalculateGroupNum() override;
    virtual void Clear() override;

public:
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_TargetTex = _Tex; }
    void SetClearColor(Vec4 _Color) { m_ClearColor = _Color; }

private:
    Ptr<CTexture>       m_TargetTex;
    Vec4                m_ClearColor;

};

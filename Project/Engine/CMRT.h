#ifndef CMRT_H
#define CMRT_H

#include "CEntity.h"

#include "CTexture.h"

class CMRT :
    public CEntity
{
public:
    CLONE_DISABLE(CMRT);
    CMRT();
    ~CMRT();

public:
    void Create(int _RTCount, Ptr<CTexture>* _arrRT, Ptr<CTexture> _DSTex);
    void SetClearColor(Vec4* _Color, bool _Independent);
    void OMSet();

    void Clear() { ClearRenderTargetView(); ClearDepthStencilView(); }
    void ClearRenderTargetView();
    void ClearDepthStencilView();

    Ptr<CTexture> GetRT(int _Index) { return m_arrRT[_Index]; }

private:
    Ptr<CTexture>   m_arrRT[8]; // 최대 설정 가능한 RenderTarget 개수가 8개
    Vec4            m_ClearColor[8];
    int             m_RTCount;

    Ptr<CTexture>   m_DSTex;    // 깊이 텍스쳐, 없어도 가능

    D3D11_VIEWPORT  m_ViewPort; // MRT 뷰포트 설정 값

};

#endif
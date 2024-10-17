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
    Ptr<CTexture>   m_arrRT[8]; // �ִ� ���� ������ RenderTarget ������ 8��
    Vec4            m_ClearColor[8];
    int             m_RTCount;

    Ptr<CTexture>   m_DSTex;    // ���� �ؽ���, ��� ����

    D3D11_VIEWPORT  m_ViewPort; // MRT ����Ʈ ���� ��

};

#endif
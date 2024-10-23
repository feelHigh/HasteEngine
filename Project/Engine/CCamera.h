#ifndef CCAMERA_H
#define CCAMERA_H

#include "CComponent.h"

class CGameObject;

enum PROJECTION_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,
};

class CCamera :
    public CComponent
{
public:
    CLONE(CCamera);
    CCamera();
    ~CCamera();

    friend class CRenderMgr;

public:
    virtual void Begin() override;
    virtual void FinalTick() override;

public:
    void Render();

public:
    void SetPriority(int _Priority) { m_Priority = _Priority; }
    void SetLayer(UINT _LayerIdx, bool _bCheck)
    {
        if (_bCheck)
            m_LayerCheck |= (1 << _LayerIdx);
        else
            m_LayerCheck &= ~(1 << _LayerIdx);
    }
    void SetLayerAll() { m_LayerCheck = 0xffffffff; }

    bool GetLayerCheck(UINT _LayerIdx) { return m_LayerCheck & (1 << _LayerIdx); }


    void SetProjectionType(PROJECTION_TYPE _Type) { m_ProjType = _Type; }
    PROJECTION_TYPE GetProjectionType() { return m_ProjType; }

    void SetWidth(float _Width)
    {
        m_Width = _Width;
        m_AspectRatio = m_Width / m_Height;
    }

    void SetHeight(float _Height)
    {
        m_Height = _Height;
        m_AspectRatio = m_Width / m_Height;
    }

    float GetWidth() { return m_Width; }
    float GetHeight() { return m_Height; }
    float GetAspectRatio() { return m_AspectRatio; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }

    void SetFOV(float _FOV) { m_FOV = _FOV; }
    float GetFOV() { return m_FOV; }

    void SetScale(float _Scale) { m_ProjectionScale = _Scale; }
    float GetScale() { return m_ProjectionScale; }

    const Matrix& GetViewMatrix() { return m_matView; }
    const Matrix& GetViewMatrixInverse() { return m_matViewInv; }
    const Matrix& GetProjectionMatrix() { return m_matProj; }
    const Matrix& GetProjectionMatrixInverse() { return m_matProjInv; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void SortGameObject();

    void Render_Deferred();
    void Render_Opaque();
    void Render_Masked();
    void Render_Transparent();
    void Render_Effect();
    void Render_Particle();
    void Render_Postprocess();
    void Render_UI();

    void Clear();

private:
    int                     m_Priority;
    UINT                    m_LayerCheck;       // ���ϴ� ���̾ ī�޶� �������� ����

    PROJECTION_TYPE         m_ProjType;

    float                   m_Width;
    float                   m_Height;
    float                   m_AspectRatio;      // ��Ⱦ ��
    float                   m_Far;              // ī�޶� �� �� �ִ� �þ� �Ÿ�
    float                   m_FOV;              // Field Of View (�þ� ����, �þ� ��)
    float                   m_ProjectionScale;

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    vector<CGameObject*>    m_vecDeferred;      // Deferred
    vector<CGameObject*>    m_vecOpaque;        // ������
    vector<CGameObject*>    m_vecMasked;        // ������, ����
    vector<CGameObject*>    m_vecTransparent;   // ����, ������
    vector<CGameObject*>    m_vecEffect;
    vector<CGameObject*>    m_vecParticles;     // ����, ������, ���� Ÿ��
    vector<CGameObject*>    m_vecPostProcess;   // ��ó�� ������Ʈ
    vector<CGameObject*>    m_vecUI;            // ��ó�� ������Ʈ

};

#endif
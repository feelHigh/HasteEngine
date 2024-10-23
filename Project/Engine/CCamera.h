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
    UINT                    m_LayerCheck;       // 원하는 레이어만 카메라에 찍히도록 설정

    PROJECTION_TYPE         m_ProjType;

    float                   m_Width;
    float                   m_Height;
    float                   m_AspectRatio;      // 종횡 비
    float                   m_Far;              // 카메라가 볼 수 있는 시야 거리
    float                   m_FOV;              // Field Of View (시야 범위, 시야 각)
    float                   m_ProjectionScale;

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    vector<CGameObject*>    m_vecDeferred;      // Deferred
    vector<CGameObject*>    m_vecOpaque;        // 불투명
    vector<CGameObject*>    m_vecMasked;        // 불투명, 투명
    vector<CGameObject*>    m_vecTransparent;   // 투명, 반투명
    vector<CGameObject*>    m_vecEffect;
    vector<CGameObject*>    m_vecParticles;     // 투명, 반투명, 입자 타입
    vector<CGameObject*>    m_vecPostProcess;   // 후처리 오브젝트
    vector<CGameObject*>    m_vecUI;            // 후처리 오브젝트

};

#endif
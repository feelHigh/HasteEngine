#ifndef CCOLLIDER2D_H
#define CCOLLIDER2D_H

#include "CComponent.h"

class CCollider2D :
    public CComponent
{
public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();

public:
    virtual void FinalTick() override;

    virtual void BeginOverlap(CCollider2D* _Other);
    virtual void Overlap(CCollider2D* _Other);
    virtual void EndOverlap(CCollider2D* _Other);

public:
    void SetOffset(Vec3 _Offset) { m_Offset = _Offset; }
    void SetScale(Vec3 _Scale) { m_Scale = _Scale; }
    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set; }

    Vec3 GetOffset() { return m_Offset; }
    Vec3 GetScale() { return m_Scale; }
    Vec3 GetWorldPosition() { return m_matColWorld.Translation(); }
    const Matrix& GetWorldMatrix() { return m_matColWorld; }

    int GetOverlapCount() { return m_OverlapCount; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Vec3    m_Offset;
    Vec3    m_Scale;            // 배율 or 절대 크기
    Matrix  m_matColWorld;      // 충돌체의 최종 월드 상태
    int     m_OverlapCount;

    bool    m_IndependentScale; // 오브젝트의 크기에 영향을 받을지 여부

};

#endif
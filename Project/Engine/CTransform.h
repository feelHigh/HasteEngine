#ifndef CTRANSFORM_H
#define CTRANSFORM_H

#include "CComponent.h"

class CTransform :
    public CComponent
{
public:
    CLONE(CTransform);
    CTransform();
    ~CTransform();

public:
    virtual void FinalTick() override;
    void Binding();

public:
    void SetRelativePosition(Vec3 _Pos) { m_RelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rot) { m_RelativeRot = _Rot; }

    void SetRelativePosition(float x, float y, float z) { m_RelativePos = Vec3(x, y, z); }
    void SetRelativeScale(float x, float y, float z) { m_RelativeScale = Vec3(x, y, z); }
    void SetRelativeRotation(float x, float y, float z) { m_RelativeRot = Vec3(x, y, z); }

    void SetWorldMatrix(const Matrix& matWorld) { m_matWorld = matWorld; }

    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set; }
    bool IsIndependentScale() { return m_IndependentScale; }

    Vec3 GetRelativePosition() { return m_RelativePos; }
    Vec3 GetWorldPosition() { return m_matWorld.Translation(); }

    Vec3 GetRelativeScale() { return m_RelativeScale; }
    Vec3 GetWorldScale();

    Vec3 GetRelativeRoatation() { return m_RelativeRot; }
    Vec3 GetRelativeDirection(DIRECTION_TYPE _Type) { return m_RelativeDir[_Type]; }
    Vec3 GetWorldDirection(DIRECTION_TYPE _Type) { return m_WorldDir[_Type]; }

    const Matrix& GetWorldMat() { return m_matWorld; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Vec3    m_RelativePos;
    Vec3    m_RelativeScale;
    Vec3    m_RelativeRot;
    Vec3    m_RelativeDir[3];
    Vec3    m_WorldDir[3];

    Matrix  m_matWorld;         // 이동, 크기, 회전
    bool    m_IndependentScale; // 부모의 크기에 영향받지 않음

};

#endif // CTRANSFORM_H
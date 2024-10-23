#ifndef CCOMPONENT_H
#define CCOMPONENT_H

#include "CEntity.h"
#include "CGameObject.h"

class CComponent :
    public CEntity
{
public:
    CLONE_DISABLE(CComponent);
    CComponent(COMPONENT_TYPE _Type);
    CComponent(const CComponent& _Origin);
    ~CComponent();

    friend class CGameObject;

public:
    virtual void Initialize() {}
    virtual void Begin();
    virtual void Tick();
    virtual void FinalTick() = 0;

public:
    COMPONENT_TYPE GetComponentType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

public:
    virtual void SaveToFile(FILE* _File) = 0;   // 파일에 컴포넌트의 정보를 저장
    virtual void LoadFromFile(FILE* _File) = 0; // 파일에 컴포넌트의 정보를 로드

public:
    // Default Components
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(FlipbookComponent);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(Light3D);
    // Render Components
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(ParticleSystem);
    GET_OTHER_COMPONENT(Decal);
    GET_OTHER_COMPONENT(Skybox);

private:
    const COMPONENT_TYPE    m_Type;
    CGameObject*            m_Owner;

private:
    void SetOwner(CGameObject* _Object) { m_Owner = _Object; }

};

#endif
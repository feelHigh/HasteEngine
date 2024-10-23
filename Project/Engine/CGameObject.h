#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H

#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;

class CGameObject :
    public CEntity
{
public:
    CLONE(CGameObject);
    CGameObject();
    CGameObject(const CGameObject& _Origin);
    ~CGameObject();

    friend class CLevel;
    friend class CLayer;
    friend class CTaskMgr;

public:
    void Begin();
    void Tick();
    virtual void FinalTick();
    void Render();

public:
    void AddComponent(CComponent* _Comopnent);
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    CGameObject* GetParent() { return m_Parent; }
    int GetLayerIdx() { return m_LayerIdx; }
    void AddChild(CGameObject* _ChildObject);
    const vector<CGameObject*>& GetChildren() { return m_vecChildren; }
    const vector<CScript*> GetScripts() { return m_vecScript; }

    bool IsAncestor(CGameObject* _Object);
    bool IsDead() { return m_Dead; }

    void DisconnectWithLayer();

    // Unregister : 등록을 취소하다, 등록하는 행위를 취소하다
    // Deregister : 등록된것을 취소하다
    void DeregisterChild();

public:
    // Default Components
    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(FlipbookComponent, FLIPBOOKCOMPONENT);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Light3D, LIGHT3D);
    // Render Components
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(Decal, DECAL);
    GET_COMPONENT(Skybox, SKYBOX);

private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;
    vector<CScript*>        m_vecScript;

    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChildren;

    int                     m_LayerIdx; // 소속 레이어의 인덱스 번호
    bool                    m_Dead;

};

#endif
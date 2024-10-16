#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
public:
    CLONE(CLevel);
    CLevel();
    CLevel(const CLevel& _Origin);
    ~CLevel();

    friend class CTaskMgr;
    
public:
    void Begin();   // ������ ���۵Ǵ� ���� or ������ �߰�(�շ�) �ϴ� ����
    void Tick();
    void FinalTick();
    void ClearObject();

public:
    void AddObject(int LayerIdx, CGameObject* _Object, bool _bMoveChild = false);
    void RegisterAsParent(int LayerIdx, CGameObject* _Object);

    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    LEVEL_STATE GetState() { return m_State; }
    CGameObject* FindObjectByName(const wstring& _Name);

private:
    void ChangeState(LEVEL_STATE _NextState);

private:
    CLayer* m_Layer[MAX_LAYER];
    LEVEL_STATE     m_State;

};

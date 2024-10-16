#pragma once

#include "CEditor.h"

class CComponentUI :
    public CEditor
{
public:
    CComponentUI(COMPONENT_TYPE _Type);
    ~CComponentUI();

public:
    void SetTargetObject(CGameObject* _Object);
    CGameObject* GetTargetObject() { return m_TargetObject; }

    COMPONENT_TYPE GetComponentUIType() { return m_Type; }

private:
    class CGameObject* m_TargetObject;
    const COMPONENT_TYPE    m_Type;

protected:
    void Title();

};

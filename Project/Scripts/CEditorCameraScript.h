#pragma once
#include <Engine/CScript.h>

class CEditorCameraScript :
    public CScript
{
public:
    CLONE(CEditorCameraScript);
    CEditorCameraScript();
    ~CEditorCameraScript();

public:
    virtual void Tick() override;

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File)override {}

private:
    void OrthographicMove();
    void PerspectiveMove();

private:
    float   m_Speed;

};

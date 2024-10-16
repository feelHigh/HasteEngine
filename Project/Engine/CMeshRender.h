#ifndef CMESHRENDER_H
#define CMESHRENDER_H

#include "CRenderComponent.h"

class CMeshRender :
    public CRenderComponent
{
public:
    CLONE(CMeshRender);
    CMeshRender();
    ~CMeshRender();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

};

#endif
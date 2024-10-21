#pragma once
#include "CEditor.h"

class CLevelEditor :
    public CEditor
{
public:
    CLevelEditor();
    virtual ~CLevelEditor();

public:
	virtual void Initialize() override;
	virtual void Tick() override;
	virtual void Update() override;
	virtual void Render() override;

private:

};

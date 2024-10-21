#pragma once

class CEditor
{
public:
	CEditor(EDITOR_TYPE _EditorType);
	virtual ~CEditor();

public:
	virtual void Initialize() = 0;
	virtual void Tick() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	EDITOR_TYPE m_EditorType;

};

#pragma once

class CLevel;
class CGameObject;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
public:
	void Initialize();
	void Progress();

	friend class CTaskMgr;

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	CGameObject* FindObjectByName(const wstring& _Name);

	void LevelChanged();
	bool IsLevelChanged() { return m_LevelChanged; }

private:
	bool ChangeLevel(CLevel* _NextLevel);

private:
	CLevel*		m_CurLevel;
	bool		m_LevelChanged;

};

#pragma once

typedef void(*OBJECT_SAVE)(FILE*, class CGameObject*);
typedef CGameObject* (*OBJECT_LOAD)(FILE*);

class CEngine
	: public CSingleton<CEngine>
{
	SINGLE(CEngine);
public:
	int Initialize(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _Func);
	void Progress();
	void ChangeWindowScale(UINT _Width, UINT _Height);
	FMOD::System* GetFMODSystem() { return m_FMODSystem; }

public:
	HWND GetMainWnd() { return m_hWnd; }

private:
	HWND			m_hWnd;
	POINT			m_ptResolution;
	FMOD::System* m_FMODSystem;  // FMOD 관리자 클래스

};

#pragma once

class CTimeMgr
	: public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr)
public:
	void Initialize();
	void Tick();
	void Render();

public:
	float GetDeltaTime() { return m_DeltaTime; }
	float GetTime() { return m_Time; }

	float GetEngineDeltaTime() { return m_E_DeltaTime; }
	float GetEngineTime() { return m_E_Time; }

	UINT GetFPS() { return m_FPS; }

private:
	// LARGE_INTEGER - 8 ����Ʈ ���� ���
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_FPS;

	float			m_DeltaTime;	// ������ ���� �ð�( 1 ������ �����ϴµ� �ɸ��� �ð� )
	float			m_Time;			// ���α׷��� ���� ���ķ� ����� �ð�

	float			m_E_DeltaTime;	// ������ ���� �ð�( 1 ������ �����ϴµ� �ɸ��� �ð� )
	float			m_E_Time;		// ���α׷��� ���� ���ķ� ����� �ð�

	wchar_t			m_TimeInfo[255];

};
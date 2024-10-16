#ifndef CTASKMGR_H
#define CTASKMGR_H

class CTaskMgr
	: public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr)
public:
	void Tick();

public:
	void AddTask(const tTask& _Task);

private:
	void ClearGC();
	void ExecuteTask();

private:
	vector<tTask>			m_vecTask;
	vector<CGameObject*>	m_GC;	// Garbage Collector

};

#endif // CTASKMGR_H
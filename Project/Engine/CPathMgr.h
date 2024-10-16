#ifndef CPATHMGR_H
#define CPATHMGR_H

class CPathMgr
	: public CSingleton<CPathMgr>
{
	SINGLE(CPathMgr);
public:
	void Initialize();

private:
	void GetParentPath(_Inout_ wchar_t* _Buffer);

public:
	const wstring& GetContentPath() { return m_Content; }
	const wstring& GetSolutionPath() { return m_Solution; }
	wstring GetRelativePath(const wstring& _FilePath);

private:
	wstring		m_Content;
	wstring		m_Solution;

};

#endif
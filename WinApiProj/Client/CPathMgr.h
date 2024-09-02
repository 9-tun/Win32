#pragma once
class CPathMgr
{
	SINGLE(CPathMgr);
	
private:
	wchar_t		m_szContentPath[255];	// 경로를 저장할 문자열 255글자로 제한, 윈도우에서 경로가 255글자로 제한이 걸려있음.
	wchar_t		m_szRelativePath[255];	// 상대 경로


public:
	void init();
	const wchar_t* GetContentPath() { return m_szContentPath; }
	wstring GetRelativePath(const wchar_t* _filepath);
};


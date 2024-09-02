#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
{}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::init()
{
	GetCurrentDirectory(255, m_szContentPath);
	
	// 상위 폴더로
	int iLen = (int)wcslen(m_szContentPath);
	for (int i = iLen - 1; 0 <= i; --i)		// 뒤에서 부터 '\\'를 찾아서 '\0'으로 변경 -> 상위 폴더 경로명
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	// 문자열 이어 붙여주기
	wcscat_s(m_szContentPath, 255, L"\\bin\\Content\\");
	//SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);

}

wstring CPathMgr::GetRelativePath(const wchar_t * _filepath)
{
	wstring strFilePath = _filepath;

	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFulllen = strFilePath.length();

	wstring strRelativePath = strFilePath.substr(iAbsLen, iFulllen - iAbsLen);

	return strRelativePath;
}

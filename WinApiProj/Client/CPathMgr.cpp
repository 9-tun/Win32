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
	
	// ���� ������
	int iLen = (int)wcslen(m_szContentPath);
	for (int i = iLen - 1; 0 <= i; --i)		// �ڿ��� ���� '\\'�� ã�Ƽ� '\0'���� ���� -> ���� ���� ��θ�
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	// ���ڿ� �̾� �ٿ��ֱ�
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

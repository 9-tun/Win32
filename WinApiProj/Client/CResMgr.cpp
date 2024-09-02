#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"
#include "CSound.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapTex);
}

CTexture* CResMgr::LoadTexture(const wstring & _strKey, const wstring & _strRelativePath)
{
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();	// �⺻ ��� ���
	strFilePath += _strRelativePath;								// �⺻ ��ο� �Ű����� ��� ���ϱ�

	pTex = new CTexture;						// map second�� ���� CTexture ��ü ���� �Ҵ�
	pTex->Load(strFilePath);					// DC, ��Ʈ��, ��Ʈ�� ���� Load
	pTex->SetKey(_strKey);						// ��� ����
	pTex->SetRelativePath(_strRelativePath);	// ��� ����
	
	m_mapTex.insert(make_pair(_strKey, pTex));	// Ű �� - CTexture ��Ī

	return pTex;
}

CTexture * CResMgr::CreateTexture(const wstring & _strKey, UINT _iWidth, UINT _iHeight)
{
	// ������ Key���� �ִ��� ���� Ȯ��
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}
	
	pTex = new CTexture;						// map second�� ���� CTexture ��ü ���� �Ҵ�
	pTex->Create(_iWidth, _iHeight);			// ����, ���� ���� ������� Texture ����
	pTex->SetKey(_strKey);						// ��� ����

	m_mapTex.insert(make_pair(_strKey, pTex));	// Ű �� - CTexture ��Ī

	return pTex;
}

CTexture* CResMgr::FindTexture(const wstring & _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey);
	if (iter == m_mapTex.end())		// ���� ã�� ���ߴٸ� iterator�� end iterator�� �ȴ�.
	{
		return nullptr;
	}
	return (CTexture*)iter->second;			// ���� ã�Ҵٸ� iterator�� second�� �츮�� ã�� CTexture*��
}

CSound * CResMgr::LoadSound(const wstring & _strKey, const wstring & _strRelativePath)
{
	CSound* pSound = FindSound(_strKey);
	if (nullptr != pSound)
	{
		return pSound;
	}

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();	// �⺻ ��� ���
	strFilePath += _strRelativePath;								// �⺻ ��ο� �Ű����� ��� ���ϱ�

	pSound = new CSound;
	pSound->Load(strFilePath);					
	pSound->SetKey(_strKey);						
	pSound->SetRelativePath(_strRelativePath);	

	m_mapSound.insert(make_pair(_strKey, pSound));	

	return pSound;
}

CSound * CResMgr::FindSound(const wstring & _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapSound.find(_strKey);
	if (iter == m_mapSound.end())		// ���� ã�� ���ߴٸ� iterator�� end iterator�� �ȴ�.
	{
		return nullptr;
	}
	return (CSound*)iter->second;
}

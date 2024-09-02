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

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();	// 기본 경로 얻기
	strFilePath += _strRelativePath;								// 기본 경로에 매개인자 경로 더하기

	pTex = new CTexture;						// map second에 넣을 CTexture 객체 동적 할당
	pTex->Load(strFilePath);					// DC, 비트맵, 비트맵 정보 Load
	pTex->SetKey(_strKey);						// 멤버 저장
	pTex->SetRelativePath(_strRelativePath);	// 멤버 저장
	
	m_mapTex.insert(make_pair(_strKey, pTex));	// 키 값 - CTexture 매칭

	return pTex;
}

CTexture * CResMgr::CreateTexture(const wstring & _strKey, UINT _iWidth, UINT _iHeight)
{
	// 동일한 Key값이 있는지 먼저 확인
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}
	
	pTex = new CTexture;						// map second에 넣을 CTexture 객체 동적 할당
	pTex->Create(_iWidth, _iHeight);			// 가로, 세로 길이 기반으로 Texture 생성
	pTex->SetKey(_strKey);						// 멤버 저장

	m_mapTex.insert(make_pair(_strKey, pTex));	// 키 값 - CTexture 매칭

	return pTex;
}

CTexture* CResMgr::FindTexture(const wstring & _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey);
	if (iter == m_mapTex.end())		// 만약 찾지 못했다면 iterator는 end iterator가 된다.
	{
		return nullptr;
	}
	return (CTexture*)iter->second;			// 만약 찾았다면 iterator의 second가 우리가 찾던 CTexture*임
}

CSound * CResMgr::LoadSound(const wstring & _strKey, const wstring & _strRelativePath)
{
	CSound* pSound = FindSound(_strKey);
	if (nullptr != pSound)
	{
		return pSound;
	}

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();	// 기본 경로 얻기
	strFilePath += _strRelativePath;								// 기본 경로에 매개인자 경로 더하기

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
	if (iter == m_mapSound.end())		// 만약 찾지 못했다면 iterator는 end iterator가 된다.
	{
		return nullptr;
	}
	return (CSound*)iter->second;
}

#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CEventMgr.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (nullptr != m_arrScene[i])
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneMgr::init()
{
	// Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;			// 씬 목록 배열 index 1에 CScene_Start 추가
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");	// CScene_Start의 이름을 "Start Scene"으로 지정

	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");

	//m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_Stage_01;
	//m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_Stage_02;
	 
	// 현재 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];				// CScene_Start를 CSceneMgr의 m_pCurScene로 가리키기
	m_pCurScene->Enter();											// 씬 진입 첫 화면에 등장할 객체 추가 + 위치 잡아주기
}

void CSceneMgr::update()
{
	m_pCurScene->update ();		// 객체 위치 Update
	m_pCurScene->finalupdate();	// 매 프레임마다 내가 직접 관리하지 않아도 항상 처리되어야 하는 것들 update (예, CCollider는 객체를 알아서 따라 다녀야 함)
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	m_pCurScene->Exit();					// 씬을 벗어나고
	m_pCurScene = m_arrScene[(UINT)_eNext];
	m_pCurScene->Enter();					// 씬에 진입하고

}


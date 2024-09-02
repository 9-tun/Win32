#include "pch.h"
#include "CEventMgr.h"

#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"

#include "AI.h"
#include "CState.h"

#include "CUIMgr.h"

CEventMgr::CEventMgr()
{

}
CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	// ================================================
	// 이전 프레임에서 등록해 둔 Dead Object들을 삭제한다.
	// ================================================
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];	// delete는 C++에서 동적으로 할당된 메모리를 해제
	}
	m_vecDead.clear();			// 벡터 내 포인터를 제거하지만, 포인터가 가리키는 객체 자체를 삭제하지 않습니다.



	// ==========
	// Event 처리
	// ==========
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Excute(m_vecEvent[i]);
	}		
	m_vecEvent.clear();		// 모든 이벤트 실행을 마치고 나서 이벤트 목록 비워주기
}

void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEven)
	{ 
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam: Object Address
		// wParam: Object Group Type
		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;
		
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam: Obejct address
		// Obejct를 Dead 상태로 변경
		// 삭제 예정 Object들을 가변 배열 m_vecDead에 모아둔다.
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();			// 객체를 죽은 상태로 만들고
		m_vecDead.push_back(pDeadObj);	// 가변 배열에 모아두기
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam: Next Scene Type
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

		// 포커스 UI 해제 (이전 Scene의 UI를 가리키고 있기 때문)
		CUIMgr::GetInst()->SetFocusedUI(nullptr);
	}
		break;
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		// lParam: AI
		// wParam: Next Type
		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eNextSate = (MON_STATE)_eve.wParam;
		pAI->ChangeState(eNextSate);
	}
		break;
	}
}

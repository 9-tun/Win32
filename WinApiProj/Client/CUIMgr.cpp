#include "pch.h"
#include "CUIMgr.h"

#include "CScene.h"
#include "CSceneMgr.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_pFocusedUI(nullptr)
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::update()
{
	// 1. FocusedUI 확인
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI) // m_pFocusedUI가 nullptr 이면 반환
		return;

	bool bLbtnTap = KEY_TAP(KEY::LBTN);		// 마우스 좌버튼이 이번 프레임에서 눌렸는지 확인
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);	// 마우스 좌버튼을 이번 프레임에서 딱 뗌

	// 2. FoucusedUI 내에서, 부모 UI 포함, 자식 UI들 중 실제 타겟팅된 UI를 가져온다.
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);	// 부모 UI를 건네고 나오는 게 자식 UI

	if (nullptr != pTargetUI)
	{
		pTargetUI->MouseOn();
		if (bLbtnTap)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLbtnUp();
			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLbtnClicked();
			}
			// 왼쪽버튼 떼면, 눌렸던 체크 다시 해제한다.
			pTargetUI->m_bLbtnDown = false;
		}
	}	
}

void CUIMgr::SetFocusedUI(CUI * _pUI)
{
	// 이미 포커싱 중인 경우 or 포커싱 해제 요청인 경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI; 
		return;
	}	

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}
	// 벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(iter);		// 지우고
	vecUI.push_back(m_pFocusedUI);	// 맨 뒤로 넣어주기
}

CUI * CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup(); 

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	// 기존 포커싱 UI를 받아두고 변경되었는지 확인한다.
	CUI* pFocusedUI = m_pFocusedUI;

	if (!bLbtnTap)	// 마우스 클릭 자체가 없었으면 Focus에는 변화 없음
	{
		return pFocusedUI;
	}

	// 왼쪽버튼 TAP이 발생했다는 전제
	vector<CObject*>::iterator targetiter = vecUI.end();;
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
	}

	// 이번에 Focus된 UI가 없다.
	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetiter;

	// 벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(targetiter);		// 지우고
	vecUI.push_back(pFocusedUI);	// 맨 뒤로 넣어주기

	return pFocusedUI;
}

CUI * CUIMgr::GetTargetedUI(CUI * _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 1. 부모 UI 포함, 모든 자식들을 검사한다.
	CUI* pTargetUI = nullptr;

	// 자주 쓰는 queue랑 vecNoneTarget은 정적 멤버로 선언하자
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;	

	// 이전 데이터가 남아있지 않도록 clear 해주기
	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);	// 리스트를 큐처럼 사용, 부모 노드를 넣고 꺼내면 해당 부모의 자식 노드를 큐에 추가 그럼 Level 순회가 구현됨
	
	while (!queue.empty())
	{
		// 큐에서 데이터 하나 꺼내기
		CUI* pUI = queue.front();	// 큐 데이터 꺼내기
		queue.pop_front();			// 큐 데이터 지우기

		// 큐에서 꺼내온 UI가 TargetUI인지 확인
		// 타겟 UI들 중, 더 낮은 계층의 자식 UI가 우선순위가 더 높음
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)		// 앞서 타겟으로 잡혀 있던 애가 있으면 걔는 NoneTarget으로 넣기
			{
				vecNoneTarget.push_back(pTargetUI);
			}			
			pTargetUI = pUI;				// 그리고 이번에 꺼낸 애를 새 Target으로 설정
		}
		else
		{
			vecNoneTarget.push_back(pUI);	// 이번에 꺼낸 애가 MouseOn이 아니라면 NoneTarget에 넣기
		}

		// 자식 넣어주기
		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}

	// Target UI를 제외한 나머지 UI들에 대해서 왼쪽버튼 떼어졌으면, 눌렸던 체크를 다시 해제한다.
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}

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
	// ���� �����ӿ��� ����� �� Dead Object���� �����Ѵ�.
	// ================================================
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];	// delete�� C++���� �������� �Ҵ�� �޸𸮸� ����
	}
	m_vecDead.clear();			// ���� �� �����͸� ����������, �����Ͱ� ����Ű�� ��ü ��ü�� �������� �ʽ��ϴ�.



	// ==========
	// Event ó��
	// ==========
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Excute(m_vecEvent[i]);
	}		
	m_vecEvent.clear();		// ��� �̺�Ʈ ������ ��ġ�� ���� �̺�Ʈ ��� ����ֱ�
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
		// Obejct�� Dead ���·� ����
		// ���� ���� Object���� ���� �迭 m_vecDead�� ��Ƶд�.
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();			// ��ü�� ���� ���·� �����
		m_vecDead.push_back(pDeadObj);	// ���� �迭�� ��Ƶα�
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam: Next Scene Type
		CSceneMgr::GetInst()->ChangeScene((SCENE_TYPE)_eve.lParam);

		// ��Ŀ�� UI ���� (���� Scene�� UI�� ����Ű�� �ֱ� ����)
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

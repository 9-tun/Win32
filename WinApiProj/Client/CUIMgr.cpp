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
	// 1. FocusedUI Ȯ��
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI) // m_pFocusedUI�� nullptr �̸� ��ȯ
		return;

	bool bLbtnTap = KEY_TAP(KEY::LBTN);		// ���콺 �¹�ư�� �̹� �����ӿ��� ���ȴ��� Ȯ��
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);	// ���콺 �¹�ư�� �̹� �����ӿ��� �� ��

	// 2. FoucusedUI ������, �θ� UI ����, �ڽ� UI�� �� ���� Ÿ���õ� UI�� �����´�.
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);	// �θ� UI�� �ǳװ� ������ �� �ڽ� UI

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
			// ���ʹ�ư ����, ���ȴ� üũ �ٽ� �����Ѵ�.
			pTargetUI->m_bLbtnDown = false;
		}
	}	
}

void CUIMgr::SetFocusedUI(CUI * _pUI)
{
	// �̹� ��Ŀ�� ���� ��� or ��Ŀ�� ���� ��û�� ���
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
	// ���� ������ �� �ڷ� ���� ��ü
	vecUI.erase(iter);		// �����
	vecUI.push_back(m_pFocusedUI);	// �� �ڷ� �־��ֱ�
}

CUI * CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup(); 

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	// ���� ��Ŀ�� UI�� �޾Ƶΰ� ����Ǿ����� Ȯ���Ѵ�.
	CUI* pFocusedUI = m_pFocusedUI;

	if (!bLbtnTap)	// ���콺 Ŭ�� ��ü�� �������� Focus���� ��ȭ ����
	{
		return pFocusedUI;
	}

	// ���ʹ�ư TAP�� �߻��ߴٴ� ����
	vector<CObject*>::iterator targetiter = vecUI.end();;
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
	}

	// �̹��� Focus�� UI�� ����.
	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetiter;

	// ���� ������ �� �ڷ� ���� ��ü
	vecUI.erase(targetiter);		// �����
	vecUI.push_back(pFocusedUI);	// �� �ڷ� �־��ֱ�

	return pFocusedUI;
}

CUI * CUIMgr::GetTargetedUI(CUI * _pParentUI)
{
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 1. �θ� UI ����, ��� �ڽĵ��� �˻��Ѵ�.
	CUI* pTargetUI = nullptr;

	// ���� ���� queue�� vecNoneTarget�� ���� ����� ��������
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;	

	// ���� �����Ͱ� �������� �ʵ��� clear ���ֱ�
	queue.clear();
	vecNoneTarget.clear();

	queue.push_back(_pParentUI);	// ����Ʈ�� ťó�� ���, �θ� ��带 �ְ� ������ �ش� �θ��� �ڽ� ��带 ť�� �߰� �׷� Level ��ȸ�� ������
	
	while (!queue.empty())
	{
		// ť���� ������ �ϳ� ������
		CUI* pUI = queue.front();	// ť ������ ������
		queue.pop_front();			// ť ������ �����

		// ť���� ������ UI�� TargetUI���� Ȯ��
		// Ÿ�� UI�� ��, �� ���� ������ �ڽ� UI�� �켱������ �� ����
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)		// �ռ� Ÿ������ ���� �ִ� �ְ� ������ �´� NoneTarget���� �ֱ�
			{
				vecNoneTarget.push_back(pTargetUI);
			}			
			pTargetUI = pUI;				// �׸��� �̹��� ���� �ָ� �� Target���� ����
		}
		else
		{
			vecNoneTarget.push_back(pUI);	// �̹��� ���� �ְ� MouseOn�� �ƴ϶�� NoneTarget�� �ֱ�
		}

		// �ڽ� �־��ֱ�
		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}

	// Target UI�� ������ ������ UI�鿡 ���ؼ� ���ʹ�ư ����������, ���ȴ� üũ�� �ٽ� �����Ѵ�.
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}

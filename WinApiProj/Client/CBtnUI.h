#pragma once
#include "CUI.h"

#include "CScene.h"		// ���⸦ ���� �������� �ϴϱ� ������ ����
#include "CObject.h"	// ���⸦ ���� �������� �ϴϱ� ������ ����


typedef void(*BTN_FUNC)	(DWORD_PTR, DWORD_PTR);	//�Լ� ������ Ÿ�� ����

//void(CScene::*pMemFunc)(void); �Լ� �����͸� typedef�� ����� ���
typedef void(CScene::*SCENE_MEMFUNC)(void);	// CScene Ŭ������ ����� �ڽĵ� �Լ��� ��� ���� �� �ִ� �Լ� ������ 
typedef void(CObject::*OBJECT_MEMFUNC)(void);		// CObject Ŭ������ ����� �ڽĵ� �Լ��� ��� ���� �� �ִ� �Լ� ������

class CBtnUI :
	public CUI
{
private:
	BTN_FUNC		m_pFunc;
	DWORD_PTR		m_param1;
	DWORD_PTR		m_param2;

	SCENE_MEMFUNC	m_pSceneFunc;
	CScene*			m_pSceneInst;
	//OBJECT_MEMFUNC	m_pObjectFunc;

public:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClicked();

	void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
	{
		m_pFunc = _pFunc;
		m_param1 = _param1;
		m_param2 = _param2;
	}

	void SetClickedCallBack(CScene*	_pScene, SCENE_MEMFUNC _pSceneFunc);
	   
	CLONE(CBtnUI)

public:
	CBtnUI();
	~CBtnUI();

};


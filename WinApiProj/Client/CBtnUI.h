#pragma once
#include "CUI.h"

#include "CScene.h"		// 여기를 전방 선언으로 하니까 오류가 났음
#include "CObject.h"	// 여기를 전방 선언으로 하니까 오류가 났음


typedef void(*BTN_FUNC)	(DWORD_PTR, DWORD_PTR);	//함수 포인터 타입 정의

//void(CScene::*pMemFunc)(void); 함수 포인터를 typedef를 사용한 모습
typedef void(CScene::*SCENE_MEMFUNC)(void);	// CScene 클래스를 상속한 자식들 함수는 모두 받을 수 있는 함수 포인터 
typedef void(CObject::*OBJECT_MEMFUNC)(void);		// CObject 클래스를 상속한 자식들 함수는 모두 받을 수 있는 함수 포인터

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


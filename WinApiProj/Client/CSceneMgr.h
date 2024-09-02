#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);

private:
	// 포인터 배열은 메모리를 효율적을 관리, 왜냐면 주소값만 배열에 저장하니까
	CScene*	m_arrScene[(UINT)SCENE_TYPE::END];	// 모든 씬 목록
	CScene*	m_pCurScene;						// 현재 씬
	   
public:
	void init();
	void update();
	void render(HDC _dc);

public:
	CScene* GetCurScene() { return m_pCurScene; }

private:
	void ChangeScene(SCENE_TYPE _eNext);
	friend class CEventMgr;
};


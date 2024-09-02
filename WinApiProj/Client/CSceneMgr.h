#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);

private:
	// ������ �迭�� �޸𸮸� ȿ������ ����, �ֳĸ� �ּҰ��� �迭�� �����ϴϱ�
	CScene*	m_arrScene[(UINT)SCENE_TYPE::END];	// ��� �� ���
	CScene*	m_pCurScene;						// ���� ��
	   
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


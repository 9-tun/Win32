#pragma once
#include "CScene.h"

class CScene_Start :
	public CScene
{
private:
	Vec2	m_vForcePos;
	float	m_fForceRadius;
	float	m_fCurRadius;
	float	m_fForce;
	bool	m_bUseForce;

public:
	virtual void update();
	virtual void render(HDC _dc);

	virtual void Enter();	// 자식 클래스의 virtual은 적지 않아도 되지만, 적어주면 일반 함수와 구별을 할 수 있어서 좋다.
	virtual void Exit();

public:
	void CreateForce();

public:
	CScene_Start();
	~CScene_Start();
};


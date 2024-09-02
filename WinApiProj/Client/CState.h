#pragma once

// 걍 각 State의 공통 분모만 모아둔 Class

class AI;
class CMonster;

class CState
{
private:
	AI*			m_pAI;
	MON_STATE	m_eState;

public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType() { return m_eState; }
	CMonster* GetMonster();

public:
	virtual void update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	CState(MON_STATE _eState);	// 기본 생성자 없음
	virtual ~CState();

	friend class AI;
};


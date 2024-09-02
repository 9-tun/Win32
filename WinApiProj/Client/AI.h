#pragma once

class CMonster;
class CState;

class AI
{
private:
	map<MON_STATE, CState*>	m_mapState;		// 다양한 Monster의 State를 보유하는 map -> 씬 매니저처럼 배열로 해도 되고, map으로 해도 된다.
	CState*					m_pCurState;	// AI의 현재 상태
	CMonster*				m_pOwner;

public:
	void update();

public:
	void AddState(CState* _pState);
	CState* GetState(MON_STATE _eState);
	void SetCurState(MON_STATE _eState);

	void ChangeState(MON_STATE _eNextState);
	   
	CMonster* GetOwner() { return m_pOwner; }

public:
	AI();
	~AI();

	friend class CMonster;
};


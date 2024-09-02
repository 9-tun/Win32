#pragma once
#include "CObject.h"

struct tMonInfo
{
	float	fHP;			// 체력
	float	fSpeed;			// 속도	
	float	fRecogRange;	// 인지 범위
	float	fAttRange;		// 공격 범위
	float	fAtt;			// 공격력
};


class AI;

class CMonster :
	public CObject
{
private:
	tMonInfo	m_tInfo;
	AI*			m_pAI;

public:
	float GetSpeed() { return m_tInfo.fSpeed; }
	void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
	void SetAI(AI* _AI);
	const tMonInfo& GetInfo() { return m_tInfo; }

private:
	void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }	// Set은 아무나 할 수 없게 숨긴 대신에 CMonFactory는 친구로 선언해서 거기서는 Set을 할 수 있도록 하자

public:
	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	virtual void update();	// Monster 위치 Update
	CLONE(CMonster);

public:
	CMonster();
	~CMonster();

	friend class CMonFactory;
};


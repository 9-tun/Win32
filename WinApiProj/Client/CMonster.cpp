#include "pch.h"
#include "CMonster.h"

#include "AI.h"

#include "CtimeMgr.h"
#include "CCollider.h"

CMonster::CMonster()
	: m_tInfo{}
{
	CreateCollider();	// Monster는 충돌 기능을 가질 것이기 때문에 생성자에서 부모 Object의 CreateCollider 함수를 호출
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}
void CMonster::update()
{
	if (nullptr != m_pAI)
		m_pAI->update();
}

void CMonster::SetAI(AI * _AI)
{  
	m_pAI = _AI; 
	m_pAI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Missile_Player")
	{

	}
}

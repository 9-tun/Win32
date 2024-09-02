#include "CGround.h"

#include "CCollider.h"
#include "CGravity.h"

CGround::CGround()
{
	CreateCollider();
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CGround::update()
{
}

void CGround::OnCollisionEnter(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();		// Ground와 충돌한 객체의 위치
		Vec2 vObjScale = _pOther->GetScale();		// Ground와 충돌한 객체의 크기

		Vec2 vPos = GetCollider()->GetFinalPos();	// Ground 객체 위치
		Vec2 vScale = GetCollider()->GetScale();	// Ground 객체 크기
		
		float fLen = abs(vObjPos.y - vPos.y);						// 두 객체 간의 거리 
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;	// 두 객체가 y축으로 겹친 길이

		// 충돌을 접한 상태로 유지하기 위해서 일부러 1픽셀을 덜 올려줌
		vObjPos = pOtherObj->GetPos();	// Ground와 충돌한 객체의 위치를 
		vObjPos.y -= (fValue);	// 겹친 만큼 y축으로 올려주기

		pOtherObj->SetPos(vObjPos);
	}
}

void CGround::OnCollision(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();		// Ground와 충돌한 객체의 위치
		Vec2 vObjScale = _pOther->GetScale();		// Ground와 충돌한 객체의 크기

		Vec2 vPos = GetCollider()->GetFinalPos();	// Ground 객체 위치
		Vec2 vScale = GetCollider()->GetScale();	// Ground 객체 크기

		float fLen = abs(vObjPos.y - vPos.y);						// 두 객체 간의 거리 
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;	// 두 객체가 y축으로 겹친 길이

		// 충돌을 접한 상태로 유지하기 위해서 일부러 1픽셀을 덜 올려줌
		vObjPos = pOtherObj->GetPos();	// Ground와 충돌한 객체의 위치를 
		vObjPos.y -= (fValue);	

		pOtherObj->SetPos(vObjPos);
	}
}

void CGround::OnCollisionExit(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);
	}
}


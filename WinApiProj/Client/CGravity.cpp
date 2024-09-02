#include "CGravity.h"

#include "CObject.h"
#include "CRigidBody.h"

CGravity::CGravity()
	: m_pOwner(nullptr)
	, m_bGround(false)
{
}

CGravity::~CGravity()
{
}

void CGravity::SetGround(bool _b)
{
	m_bGround = _b;
	if (m_bGround)
	{
		Vec2 vV = m_pOwner->GetRigidBody()->GetVelocity();
		m_pOwner->GetRigidBody()->SetVelocity(Vec2 ( vV.x, 0.f));	// 기존 x축 방향 속도는 유지하고, y축 방향 속도만 0으로 세팅
	}
}

void CGravity::finalupdate()
{
	m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 800.f));
}
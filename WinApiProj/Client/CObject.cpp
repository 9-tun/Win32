#include "pch.h"
#include "CObject.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CGravity.h"

CObject::CObject()
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)	
{
}

CObject::CObject(const CObject& _origin)	// 깊은 복사 생성자 
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
{
	// CObject가 갖고 있는 Component까지 복사
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}		
	if (_origin.m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}
	if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}
}

CObject::~CObject()
{
	if (nullptr != m_pCollider)
		delete m_pCollider;	

	if (nullptr != m_pAnimator)
		delete m_pAnimator;

	if (nullptr != m_pRigidBody)
		delete m_pRigidBody;

	if (nullptr != m_pGravity)
		delete m_pGravity;
}

void CObject::finalupdate()
{
	// 각 Compnent는 Final Update만 한다.
	if (m_pAnimator)
		m_pAnimator->finalupdate();

	if (m_pGravity)					// 중력이 먼저 생성되어 있어야 RigidBody가 그걸 활용해서 Update할 수 있을 것 같다.
		m_pGravity->finalupdate();

	if (m_pRigidBody)
		m_pRigidBody->finalupdate();

	if (m_pCollider)	// 충돌체가 있다면
		m_pCollider->finalupdate();
}

void CObject::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));
	
	component_render(_dc);
}

// 충돌체 렌더를 따로 구현한 이유: 
// Player와 같은 객체는 render를 오버라이딩해서 쓰고 있다. 근데 충돌체를 호출하고자 CObject의 render를 호출하면 사각형까지 그려짐
void CObject::component_render(HDC _dc)
{
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->render(_dc);
	}
	if (nullptr != m_pCollider)
	{
		m_pCollider->render(_dc);
	}
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidBody;
	m_pRigidBody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;			// 소멸자 호출 잊지 말자
	m_pGravity->m_pOwner = this;		// Compnent Private 멤버에 접근하기 위해서 Component 쪽에서 Friends 선언을 해줘야 함
}

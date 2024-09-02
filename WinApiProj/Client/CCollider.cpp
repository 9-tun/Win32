#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCamera.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)	// ��������� ��ü�� �����ϰ� �����ϹǷ� �浹ü�� ������ ������ ID���� �ϳ��� �����ϸ鼭 ��ĥ ���� ����. (0������ ����)
	, m_iCol(0)
	, m_bActive(true)
{
}

CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr) // Owner���� �����ϴ� �� �ǹ̰� ����, �浹ü�� Owner�� ���� ���޾ƾ��� 
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
	, m_bActive(_origin.m_bActive)
{
}

CCollider::~CCollider()
{
}

void CCollider::finalupdate()
{
	// Object ��ġ�� ���� ����
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}

void CCollider::render(HDC _dc)
{
	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
		ePen = PEN_TYPE::RED;

	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));
}

void CCollider::OnCollision(CCollider * _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider * _pOther)
{
	++m_iCol;	// �浹 ���� �� count���� +1
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider * _pOther)
{
	--m_iCol;	// �浹 ���� �� count���� -1
	m_pOwner->OnCollisionExit(_pOther);
}


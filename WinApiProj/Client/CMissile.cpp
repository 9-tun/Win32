#include "pch.h"
#include "CMissile.h"

#include "CtimeMgr.h"
#include "CCollider.h"

CMissile::CMissile()
	: m_fTheta(PI/4.f)
	, m_vDir(Vec2(1.f, 1.f))
{
	m_vDir.Nomalize();
	CreateCollider();	// �Ѿ��� �浹 ����� ���� ���̱� ������ �����ڿ��� �θ� Object�� CreateCollider �Լ��� ȣ��
	GetCollider()->SetScale(Vec2(15.f, 15.f));	// �Ѿ� �浹ü ũ�� 
}

CMissile::~CMissile()
{
}

void CMissile::update()
{
	Vec2 vPos = GetPos();
	//vPos.x += 600.f * cosf(m_fTheta) * fDT;
	//vPos.y -= 600.f * sinf(m_fTheta) * fDT;
	vPos.x += 600.f * m_vDir.x * fDT;
	vPos.y += 600.f * m_vDir.y * fDT;

	SetPos(vPos); 
}

void CMissile::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);	// updated by 9tun
	Vec2 vScale = GetScale();
	Ellipse(_dc, (int)(vPos.x - vScale.x / 2.f), (int)(vPos.y - vScale.y / 2.f)
		, (int)(vPos.x + vScale.x / 2.f), (int)(vPos.y + vScale.y / 2.f));

	component_render(_dc);
}

void CMissile::OnCollisionEnter(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);	// ���⼭ ��ü�� �ٷ� �����Ǵ� ���� �ƴ϶�, �ϴ� �̺�Ʈ �Ŵ����� ��� �ξ��ٰ� ���� �����ӿ��� ���� ó�� ����
	}
}

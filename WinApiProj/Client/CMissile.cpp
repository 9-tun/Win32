#include "pch.h"
#include "CMissile.h"

#include "CtimeMgr.h"
#include "CCollider.h"

CMissile::CMissile()
	: m_fTheta(PI/4.f)
	, m_vDir(Vec2(1.f, 1.f))
{
	m_vDir.Nomalize();
	CreateCollider();	// 총알은 충돌 기능을 가질 것이기 때문에 생성자에서 부모 Object의 CreateCollider 함수를 호출
	GetCollider()->SetScale(Vec2(15.f, 15.f));	// 총알 충돌체 크기 
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
		DeleteObject(this);	// 여기서 객체가 바로 삭제되는 것이 아니라, 일단 이벤트 매니저에 모아 두었다가 다음 프레임에서 삭제 처리 예정
	}
}

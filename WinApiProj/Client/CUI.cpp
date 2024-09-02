#include "CUI.h"
#include "CCamera.h"
#include "CKeyMgr.h"

#include "SelectGDI.h"

CUI::CUI(bool _bCamAff)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
{
}

CUI::CUI(const CUI & _origin)
	: CObject(_origin)		// 복사생성자를 별도로 구현한 경우, 부모에 대한 생성자도 별도로 복사 생성자를 호출하도록 정의해야 함, 아니면 기본 생성자가 호출됨
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
{
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}	
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}

void CUI::update()
{
	// Chidl UI update
	update_child();
}

void CUI::finalupdate()
{
	// UI도 충돌체까진... 잘 모르겠지만 Animator는 가질 수 있으니까 일단 그걸 update 해주고
	CObject::finalupdate();

	// UI의 최종 좌표를 구한다.
	m_vFinalPos = GetPos(); // 부모가 없다면 본인의 offset 위치가 곧 자신의 최종 위치

	if (GetParent())	// 부모 UI가 있다면
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;	// 부모 UI위치에서 자신의 offset을 더한 것이 최종 위치
	}

	// UI Mouse 체크
	MouseOnCheck();		// Mouse가 UI위에 있는지 없는지 체크

	finalupdate_child();
}

void CUI::render(HDC _dc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);	// 절대 객체 위치를 카메라 위치로 조정하여 객체 렌더링
	}

	if (m_bLbtnDown)
	{
		SelectGDI sekect(_dc, PEN_TYPE::GREEN);
		Rectangle(_dc
			, int(vPos.x)
			, int(vPos.y)
			, int(vPos.x + vScale.x)
			, int(vPos.y + vScale.y));
	}
	else
	{
		Rectangle(_dc
			, int(vPos.x)
			, int(vPos.y)
			, int(vPos.x + vScale.x)
			, int(vPos.y + vScale.y));
	}


	// Child UI Render
	render_child(_dc);
}

void CUI::update_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void CUI::finalupdate_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->finalupdate();
	}
}

void CUI::render_child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos); // 마우스 절대 절대 좌표 위치
	}
	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}

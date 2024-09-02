#include "CPanelUI.h"
#include "CKeyMgr.h"

CPanelUI::CPanelUI()
	: CUI(false)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::update()
{
}

void CPanelUI::render(HDC _dc)
{
	CUI::render(_dc);
}

void CPanelUI::MouseOn()
{
	if (IsLbtnDown())
	{
		// 마우스 클릭한 채로 움직인 거리 값 (Drag한 이동 거리)
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		// UI 객체 위치를 Drag한 위치만큼 이동하기
		Vec2 vCurPos = GetPos();	
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;	// 시작 위치를 현재 위치로 갱신
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}
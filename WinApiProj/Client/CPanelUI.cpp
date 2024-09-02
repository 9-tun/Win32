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
		// ���콺 Ŭ���� ä�� ������ �Ÿ� �� (Drag�� �̵� �Ÿ�)
		Vec2 vDiff = MOUSE_POS - m_vDragStart;

		// UI ��ü ��ġ�� Drag�� ��ġ��ŭ �̵��ϱ�
		Vec2 vCurPos = GetPos();	
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;	// ���� ��ġ�� ���� ��ġ�� ����
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}
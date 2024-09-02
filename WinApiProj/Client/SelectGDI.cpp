#include "pch.h"
#include "SelectGDI.h"

#include "CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType)
	: m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HPEN hPen = CCore::GetInst()->GetPen(_ePenType);		// �� ����
	m_hDefaultPen = (HPEN)SelectObject(_dc, hPen);			// �� ����
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType)
	: m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hBrush = CCore::GetInst()->GetBrush(_eBrushType);	// �귯�� ����
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hBrush);			// �귯�� ����
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hDC, m_hDefaultPen);		// ���� �� �ٽ� ���� ����
	SelectObject(m_hDC, m_hDefaultBrush);	// ���� �귯�� �ٽ� ���� ����
}

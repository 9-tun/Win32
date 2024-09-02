#include "pch.h"
#include "SelectGDI.h"

#include "CCore.h"

SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _ePenType)
	: m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HPEN hPen = CCore::GetInst()->GetPen(_ePenType);		// 펜 생성
	m_hDefaultPen = (HPEN)SelectObject(_dc, hPen);			// 펜 선택
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eBrushType)
	: m_hDC(_dc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hBrush = CCore::GetInst()->GetBrush(_eBrushType);	// 브러쉬 생성
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hBrush);			// 브러쉬 선택
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hDC, m_hDefaultPen);		// 원래 펜 다시 원복 선택
	SelectObject(m_hDC, m_hDefaultBrush);	// 원래 브러쉬 다시 원복 선택
}

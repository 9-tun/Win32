#pragma once
#include "CRes.h"


class CTexture :
	public CRes
{
private:
	HDC		m_dc;
	HBITMAP m_hBit;
	BITMAP	m_bitInfo;


public:
	void Load(const wstring& _strFilePath);
	void Create(UINT _iWidth, UINT _iHeight);

	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HDC GetDC() { return m_dc; }

private:
	CTexture();
	virtual ~CTexture();

	// 오로지 CResMgr를 통해서만 CTexture 객체를 생성, 소멸할 수 있도록 하자!
	friend class CResMgr;	// 프렌드 클래스: friend로 선언된 클래스는 해당 클래스의 비공개 및 보호 멤버에 접근할 수 있습니다.

};


#pragma once

// �̱��� ����
// ��ü�� ������ 1���� ����
// ��𼭵� ���� ���� ����
//class CCore
//{
//private:
//	static CCore* g_pInst;
//
//public:
//	// ���� ��� �Լ�
//	static CCore* GetInstance()
//	{	
//		// ���� ȣ��� ���
//		if (nullptr == g_pInst)
//		{
//			g_pInst = new CCore;	// g_pInst�� Static���� ������ ������ �ְ�, new CCore�� CCore�� �������� �Ҵ��ؼ� �� �޸𸮿� �ִ�.
//		}
//		return g_pInst;
//	}
//
//	static void Release()
//	{
//		if (nullptr != g_pInst)
//			delete g_pInst;
//			g_pInst = nullptr;
//	}
//
//private:
//	CCore();
//	~CCore();
//};

//#include "pch.h"

class CTexture;

class CCore
{

public:
	// ��ü���� �����ڸ� ȣ���ϱ� ���� Static ���
	//static CCore* GetInst()
	//{
	//	static CCore core;	// ������ �޸� ���� ���

	//	// ���� ������ GetInst�Լ� �ȿ����� ���ٰ����ϴٰ� ������, �� �ּҰ��� ������ ���� ���� ����
	//	// Static ������ �����Ҵ�� �޸� ������ �Ű澲�� �ʾƵ� �ȴ�. ���α׷� �߰��� ������ �� �� ����. 
	//	return &core;
	//}
	
	SINGLE(CCore);

private:
	HWND m_hWnd;			// ���� ������ �ڵ�
	POINT m_ptResolution;	// ���� ������ �ػ�
	HDC m_hDC;				// ���� �����쿡 Draw�� DC

	CTexture*	m_pMemTex;	// ����� �ؽ���

	// ���� ����ϴ� GDI Object
	HBRUSH	m_arrBruch[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

	// �޴�
	HMENU	m_hMenu;		// Tool Scene������ �޴��� ���

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();

private:
	void Clear();
	void CreateBruchPen();

public:
	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }

	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBruch[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

};
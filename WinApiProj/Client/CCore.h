#pragma once

// 싱글톤 패턴
// 객체의 생성을 1개로 제한
// 어디서든 쉽게 접근 가능
//class CCore
//{
//private:
//	static CCore* g_pInst;
//
//public:
//	// 정적 멤버 함수
//	static CCore* GetInstance()
//	{	
//		// 최초 호출된 경우
//		if (nullptr == g_pInst)
//		{
//			g_pInst = new CCore;	// g_pInst는 Static으로 데이터 영역에 있고, new CCore의 CCore는 동적으로 할당해서 힙 메모리에 있다.
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
	// 객체없이 생성자를 호출하기 위해 Static 사용
	//static CCore* GetInst()
	//{
	//	static CCore core;	// 데이터 메모리 영역 사용

	//	// 정적 변수가 GetInst함수 안에서만 접근가능하다고 하지만, 그 주소값이 있으면 직접 접근 가능
	//	// Static 선언은 동적할당과 달리 해제를 신경쓰지 않아도 된다. 프로그램 중간에 해제를 할 수 없다. 
	//	return &core;
	//}
	
	SINGLE(CCore);

private:
	HWND m_hWnd;			// 메인 윈도우 핸들
	POINT m_ptResolution;	// 메인 윈도우 해상도
	HDC m_hDC;				// 메인 윈도우에 Draw할 DC

	CTexture*	m_pMemTex;	// 백버퍼 텍스쳐

	// 자주 사용하는 GDI Object
	HBRUSH	m_arrBruch[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

	// 메뉴
	HMENU	m_hMenu;		// Tool Scene에서만 메뉴바 사용

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
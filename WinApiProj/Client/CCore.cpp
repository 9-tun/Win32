#include "pch.h"
#include "CCore.h"

#include "CtimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CObject.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"

#include "CResMgr.h"
#include "CTexture.h"
#include "SelectGDI.h"

#include "resource.h"

#include "CSound.h"

//CCore* CCore::g_pInst = nullptr;
//CObject g_obj; 

CCore::CCore()
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_arrBruch{}
	, m_arrPen{}
	
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
	DestroyMenu(m_hMenu);
}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;	// 윈도우 핸들
	m_ptResolution = _ptResolution;	// 해상도

	// 해상도에 맞게 윈도우 크기 조정
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	// 메뉴바 생성 
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT));

	m_hDC = GetDC(_hWnd); // 지정된 윈도우(_hWnd)의 디바이스 컨텍스트(m_hDC)를 가져옵니다.
	
	// 이중 버퍼링 용도의 비트맵과 DC를 만든다. -> 화면 깜빡임 없애기 위함
	//m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y); // 디바이스 컨텍스트(m_hDC)와 호환되는 비트맵(m_hBit) 생성, 비트맵 해상도(m_ptResolution.x, m_ptResolution.y)
	//m_memDC = CreateCompatibleDC(m_hDC); // 디바이스 컨텍스트(m_hDC)와 호환되는 메모리 디바이스 컨텍스트(m_memDC)를 생성

	//HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit); // 디바이스 컨텍스트에 새 비트맵을 선택하고, 이전 비트맵 반환
	//DeleteObject(hOldBit);	// SelectObject 함수 사용 후 반환한 원래 비트맵은 삭제
	 
	// 이중 버퍼링 용도의 텍스쳐 한 장을 만든다. (위 4줄을 대체)
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer", (UINT)m_ptResolution.x, (UINT)m_ptResolution.y);
	 
	// 자주 사용할 펜 및 브러쉬 생성
	CreateBruchPen();

	// Manager 초기화
	CPathMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();

	// Sound 로드 테스트
	//CResMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\DM.wav");
	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	//pNewSound->Play();

	//pNewSound->SetPosition(50.f);	//백분률, 소리 위치 설정
	//pNewSound->PlayToBGM(true);
	//PNewSound->SetVolumn(60.f);

	return S_OK;
}


void CCore::progress()
{	
	// ==============
	// Manager Update
	// ==============
	CTimeMgr::GetInst()->update();		// 프레임이 바뀌는 데 걸리는 시간
	CKeyMgr::GetInst()->update();		// 현재 Key 상태 값, 이전 키 상태 값(bool)을 vector에 저장
	CCamera::GetInst()->update();


	// ============
	// Scene Update
	// ============
	CSceneMgr::GetInst()->update();		// 씬에 있는 객체들의 위치를 Update함

	// 충돌 체크
	CCollisionMgr::GetInst()->update();	// 씬 update 이후, 충돌 검사 진행
	   
	// UI 이벤트 체크 
	CUIMgr::GetInst()->update();

	// =========
	// Rendering
	// =========
	// 화면 Clear
	Clear();

	CSceneMgr::GetInst()->render(m_pMemTex->GetDC());		// 객체 위치를 update한 씬을 다시 화면에 그린다.
	CCamera::GetInst()->render(m_pMemTex->GetDC());			// 씬 매니저가 다 그린 다음에 Camera가 장막을 한번 더 그린다.

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY);	//m_memDC에 그린 것을 m_hDC에 붙여넣기 = 화면 송출

	CTimeMgr::GetInst()->render();	// 화면 상단에 초당 프레임 수(Frame Per Second), 델타 타임(Delta Time)-프레임 간의 시간 차이 표시



	// ===============
	// 이벤트 지연 처리
	// ===============
	CEventMgr::GetInst()->update();







}

void CCore::Clear()
{
	SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);									// 검은색 Brush를 화면을 검게 하는 데만 쓰기 위해 따로 검은색 설정하는 DC를 별도 함수로 뺌
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);	// m_memDC에 그리기
}

void CCore::CreateBruchPen()
{
	// hollow brush
	m_arrBruch[(UINT)BRUSH_TYPE::HOLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBruch[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// red blue green pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255,0,0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

void CCore::DockMenu()
{
	SetMenu(m_hWnd, m_hMenu);	// CCore::init()에서 만들어 둔 Menu를 불러와서 붙이기
	ChangeWindowSize(GetResolution(), true);
}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);	
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,(long)_vResolution.x,(long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);	// 해상도에 맞도록 창의 크기를 조정, 첫 번째 인자 LPRECT는 포인터형 반환임, 그래서 첫 번째 인자로 주소값을 전달, 마지막 인자는 메뉴바 여부
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0); // 창의 위치와 크기를 설정
}


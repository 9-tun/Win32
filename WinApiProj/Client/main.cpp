// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "global.h"
#include "framework.h"
#include "Client.h"

#include "CCore.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;									// 메인 윈도우 핸들
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//CCore* pCore= CCore::GetInstance();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 메모리 릭(누수) 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(249);	// 메모리 누수 나는 곳에 중단점을 걸 수 있다. 해당 행을 주석처리 한 채로 실행하고, 출력 결과에서 {249} 같은 게 잡히면 숫자 249를 매개인자로 입력 후 주석 해제 다시 시도하면 찾을 수 있다.

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.


	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

	// 윈도우 정보 등록    
	MyRegisterClass(hInstance);

	// 윈도우 생성
	if (!InitInstance(hInstance, nCmdShow))
	{
		return  FALSE; 
	}

	// Core 초기화
	if (FAILED(CCore::GetInst()->init(g_hWnd, POINT{ 1280, 768 })))		// 여기서 각종 Mgr클래스 init()이 호출됨
	{
		MessageBox(nullptr, L"Core 객체 초기화 실패", L"ERROR", MB_OK);
		return FALSE;
	}

	// 단축키 정보 로딩    
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	// 기본 메시지 루프입니다:
	// 메세지 큐에서 메세지 확인될 때까지 대기
	// msg.message == WM_QUIT인 경우(닫기 버튼을 누른 경우) false를 반환 -> 프로그램 종료
	// 닫기 버튼을 누르기 전까지는 while문의 loof를 돈다.

	while (true)
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) //단축키 처리가 아니면 진입
		//{
		//	TranslateMessage(&msg); // 키보드 메세지를 가공해서 문자 메세지로 변환, 큐에 메세지 생성, 키보드 입력 처리
		//	DispatchMessage(&msg);  // 메시지를 해당 창의 윈도우 프로시저로 전달(WndProc 함수)하여 처리하게 함
		//}

		// 메시지 큐에서 메시지를 확인하고 제거합니다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 메시지가 WM_QUIT인 경우 루프를 종료합니다.
			if (WM_QUIT == msg.message)
				break;
			// 메시지가 가속기(단축키)로 처리되지 않은 경우
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg); // 키보드 메시지를 번역합니다.
				DispatchMessage(&msg);  // 메시지를 해당 윈도우 프로시저(WndProc)로 전달합니다. 
			}
		}

		else
		{
			// Game 코드 수행
			// 디자인 패턴(설계유형)
			// 싱글톤 패턴
			CCore::GetInst()->progress();
		}
	} 
	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);	// 메뉴바 만드는 부분
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// 창을 생성
	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// 창 생성 실패 시, false 반환
	if (!g_hWnd)
	{
		return FALSE;
	}

	// 창을 표시하고 업데이트합니다.
	ShowWindow(g_hWnd, true);
	UpdateWindow(g_hWnd);

	// 창 생성이 성공하면 TRUE를 반환합니다.
	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// main.cpp에서는 CSene_Tool에 TitltCountProc구현한 것을 모르니, 함수가 선언되어 있음을 여기에 미리 알려둠 (마치, 헤더에 함수 정의를 쭉 써두듯)
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);	// 윈도우를 모달 방식(Focus를 가져감)으로 켜는 함수, About이 이 대화상자의 메시지 처리기임(맨 아래 있음)
				break;
			case ID_MENU_TILE:
				{
					INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_TILECOUNT), hWnd, TileCountProc);	// CSene_Tool에서 메세지 처리하는 함수 구현함
					if (iRet == IDOK)
					{

					}
				}				
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:	// 무효화 영역(Invalidate)이 발생한 경우
		{
			PAINTSTRUCT ps;

			// Device Context 만들어서 ID를 반환
			HDC hdc = BeginPaint(hWnd, &ps);	// Device Context (그리기)
			//Rectangle(hdc, 1180, 668, 1280, 768);

			// 그리기 종료
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case WM_LBUTTONDOWN:
				break;

			case WM_MOUSEMOVE:
				break;

			case WM_LBUTTONUP:
				break;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)	// Dialog 박스에서 확인 버튼을 누르거나 취소 버튼을 누르면
		{
			EndDialog(hDlg, LOWORD(wParam));						// 본인을 종료
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
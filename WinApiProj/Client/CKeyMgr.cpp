#include "pch.h"
#include "CKeyMgr.h"

#include "CCore.h"

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT,	//LEFT
	VK_RIGHT,	//RIGH,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN


	'Q',	//Q
	'W',	//W
	'E',	//E
	'R',	//R
	'T',	//T
	'Y',	//Y
	'U',	//U
	'I',	//I
	'O',	//O
	'P',	//P
	'A',	//A
	'S',	//S
	'D',	//D
	'F',	//F
	'G',	//G
	'Z',	//Z
	'X',	//X
	'C',	//C
	'V',	//V
	'B',	//B

	VK_MENU,	//ALT,
	VK_CONTROL,	//CTRL,
	VK_LSHIFT,	//LSHIFT,
	VK_SPACE,	//SPACE,
	VK_RETURN,	//ENTER,
	VK_ESCAPE,	//ESC,

	VK_LBUTTON,	// 마우스 좌측 버튼
	VK_RBUTTON,	// 마우스 우측 버튼

	//LAST,
};

CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		// 구조체를 담는 가변배열
		// 배열에 담기는 구조체 멤버 2개 세팅: 1. 현재 눌리지 않음, 2. 이전 상태 값은 전부 false 
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });		
	}
}

void CKeyMgr::update()
{
	// 윈도우 포커싱 알아내기
	//HWND hMainWnd = CCore::GetInst()->GetMainHwnd();
	HWND hWnd = GetFocus();

	// 윈도우 포커싱 중일 때 키 이벤트 동작
	if (nullptr != hWnd)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌려 있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)	// 전역 배열에 저장된 Key 값들을 하나씩 눌려있는지 확인
			{
				if (m_vecKey[i].bPrevPush)				// 이전 상태가 눌려 있었는지에 따라 eState 값 Update
				{
					// 이전에도 눌려있었으면 키 상태는 HOLD
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}
			// 키가 안 눌려있다.
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에 눌려 있었다.
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 안 눌려 있었다.
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}
		}

		// Mouse 위치 계산
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInst()->GetMainHwnd(),&ptPos);
		m_vCurMousePos = Vec2((float)ptPos.x,(float)ptPos.y);
		
	}

	// 윈도우 포커싱 해제 상태
	else
	{
		// 모든 Key 값들을 전부 NONE으로 만들어야 함
		// 단, 눌려있는 상태(TAP or HOLD)였다면 AWAY로 갔다가 NONE 으로 가야함, 그렇지 않으면 바로 NONE으로 가기
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;
			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}

	
}
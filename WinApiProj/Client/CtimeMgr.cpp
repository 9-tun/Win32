#include "pch.h"
#include "CtimeMgr.h"


#include "CCore.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_dDT(0.)
	, m_dAcc(0.)
	, m_iCallCount{}
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	// 현재 카운트
	QueryPerformanceCounter(&m_llPrevCount);	// 시스템이 부팅된 이후 고해상도 성능 카운터의 틱(tick) 수

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&m_llFrequency);	// 시스템의 성능 카운터 주파수
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 프레임 카운팅과 현재 프레임 카운팅 값의 차이를 구한다.
	// 두 카운터 값의 차이를 주파수로 나누어 경과 시간을 초 단위로 계산 -> 프레임과 프레임 사이에 걸린 시간 = 프레임이 바뀌는 데 걸리는 시간
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)(m_llFrequency.QuadPart); 

	// 이전 카운트 값을 현재 카운트 값으로 갱신 (다음 번 계산을 위해서)
	m_llPrevCount = m_llCurCount; 

// DEBUG 모두에서 중단점 걸고 코드를 확인하는 시간이 경과시간에 포함 됨, 총알이 이미 저 멀리 날아가고 없음.
// 그래서 DEBUG 모드에서 m_dDT의 최대값을 1/60으로 고정.
#ifdef _DEBUG
	if (m_dDT > (1. / 10.))
		m_dDT = (1. / 10.);
#endif
}

void CTimeMgr::render()
{
	++m_iCallCount;
	m_dAcc += m_dDT;	// DT 누적

	if (m_dAcc > 1.)	// 1초마다 한 번씩 갱신
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0;
		m_iCallCount = 0;

		wchar_t	szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS: %d, DT: %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}

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
	// ���� ī��Ʈ
	QueryPerformanceCounter(&m_llPrevCount);	// �ý����� ���õ� ���� ���ػ� ���� ī������ ƽ(tick) ��

	// �ʴ� ī��Ʈ Ƚ��
	QueryPerformanceFrequency(&m_llFrequency);	// �ý����� ���� ī���� ���ļ�
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// ���� ������ ī���ð� ���� ������ ī���� ���� ���̸� ���Ѵ�.
	// �� ī���� ���� ���̸� ���ļ��� ������ ��� �ð��� �� ������ ��� -> �����Ӱ� ������ ���̿� �ɸ� �ð� = �������� �ٲ�� �� �ɸ��� �ð�
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)(m_llFrequency.QuadPart); 

	// ���� ī��Ʈ ���� ���� ī��Ʈ ������ ���� (���� �� ����� ���ؼ�)
	m_llPrevCount = m_llCurCount; 

// DEBUG ��ο��� �ߴ��� �ɰ� �ڵ带 Ȯ���ϴ� �ð��� ����ð��� ���� ��, �Ѿ��� �̹� �� �ָ� ���ư��� ����.
// �׷��� DEBUG ��忡�� m_dDT�� �ִ밪�� 1/60���� ����.
#ifdef _DEBUG
	if (m_dDT > (1. / 10.))
		m_dDT = (1. / 10.);
#endif
}

void CTimeMgr::render()
{
	++m_iCallCount;
	m_dAcc += m_dDT;	// DT ����

	if (m_dAcc > 1.)	// 1�ʸ��� �� ���� ����
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0;
		m_iCallCount = 0;

		wchar_t	szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS: %d, DT: %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}

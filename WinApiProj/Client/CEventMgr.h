#pragma once

struct tEvent
{
	EVENT_TYPE	eEven;
	DWORD_PTR	lParam;	// DWORD_PTR은 x64, x32에 따라서 pointer 주소값을 4바이트 혹은 8바이트로 맞춰서 받음
	DWORD_PTR	wParam;
};

class CEventMgr
{
	SINGLE(CEventMgr)
private:
	vector<tEvent> m_vecEvent;
	vector<CObject*> m_vecDead;	// 삭제 예정인 Object들 모아둔 가변 배열

public:
	void update();

private:
	void Excute(const tEvent& _eve); // 이벤트를 처리해 주는 함수

public:
	void AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }

};


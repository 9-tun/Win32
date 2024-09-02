#include "pch.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

// 부모에 기본 생성자가 없는데 자식을 기본 생성자로 두면, 자식이 부모의 기본 생성자를 호출하면서 오류 발생
CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::update()
{
	// Player 위치 체크
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();	
	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터 범위 안에 들어오면 추적 상태로 전환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// 플레이어가 몬스터의 인식범위 안으로 진입
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		// Monster의 상태 값 변경
		// 하지만 이것도 프레임 update 중간에 바뀌면 객체마다 Monster의 상태를 다르게 인식할 수 있으니까 이번에 요청이 오면 다음에 바뀌는 이벤트 처리해줘야 함. func에 이벤트 구현
		// 이벤트 매니저가 처리하도록 이벤트를 모아두었다가 이를 나중에 이벤트 매니저가 update때 처리하도록 하자
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
}

void CIdleState::Enter()
{

}

void CIdleState::Exit()
{
}

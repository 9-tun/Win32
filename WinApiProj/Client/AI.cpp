#include "pch.h"
#include "AI.h"

#include "CState.h"

AI::AI()
	: m_pCurState(nullptr)
	, m_pOwner(nullptr)
{
}

AI::~AI()
{
	Safe_Delete_Map(m_mapState);
}

void AI::update()
{
	m_pCurState->update();
}

void AI::AddState(CState * _pState)
{
	// AI에 State를 넣었는데 또 넣는 것을 방지
	CState* pState = GetState(_pState->GetType());
	assert(!pState);	// nullptr가 반환된 경우에만 다음으로 진행

	// AI에 State 넣기
	m_mapState.insert(make_pair(_pState->GetType(), _pState));	// AI에 State 키-값 추가
	_pState->m_pAI = this;										// State에 AI 연결
}

CState * AI::GetState(MON_STATE _eState)
{
	map<MON_STATE, CState*>::iterator iter = m_mapState.find(_eState);
	if (iter == m_mapState.end())
		return nullptr;
	   
	return iter->second;
}

void AI::SetCurState(MON_STATE _eState)
{
	// 현재 상태 얻기
	m_pCurState = GetState(_eState);
	assert(m_pCurState);
}

void AI::ChangeState(MON_STATE _eNextState)
{
	CState* pNextState = GetState(_eNextState);
	assert(m_pCurState != pNextState);

	m_pCurState->Exit();		// 기존 State에서는 Exit
	m_pCurState = pNextState;	// 새 State를 현재 State로 설정 후
	m_pCurState->Enter();		// 현재 State로 진입
}


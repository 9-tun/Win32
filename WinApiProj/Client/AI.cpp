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
	// AI�� State�� �־��µ� �� �ִ� ���� ����
	CState* pState = GetState(_pState->GetType());
	assert(!pState);	// nullptr�� ��ȯ�� ��쿡�� �������� ����

	// AI�� State �ֱ�
	m_mapState.insert(make_pair(_pState->GetType(), _pState));	// AI�� State Ű-�� �߰�
	_pState->m_pAI = this;										// State�� AI ����
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
	// ���� ���� ���
	m_pCurState = GetState(_eState);
	assert(m_pCurState);
}

void AI::ChangeState(MON_STATE _eNextState)
{
	CState* pNextState = GetState(_eNextState);
	assert(m_pCurState != pNextState);

	m_pCurState->Exit();		// ���� State������ Exit
	m_pCurState = pNextState;	// �� State�� ���� State�� ���� ��
	m_pCurState->Enter();		// ���� State�� ����
}


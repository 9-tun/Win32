#include "pch.h"
#include "CMonFactory.h"

#include "CRigidBody.h"

#include "CMonster.h"
#include "AI.h"

#include "CIdleState.h"
#include "CTraceState.h"

CMonster * CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{	
	CMonster* pMon = nullptr;

	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;	// 몬스터 생성
		pMon->SetPos(_vPos);	// 몬스터 위치

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 150.f;

		pMon->SetMonInfo(info);

		pMon->CreateRigidBody();
		pMon->GetRigidBody()->SetMass(1.f);

		//AI* pAI = new AI;					// AI 생성
		//pAI->AddState(new CIdleState);		// Monster AI가 가질 수 있는 상태(Idle)를 map에 추가
		//pAI->AddState(new CTraceState);		// Monster AI가 가질 수 있는 상태(Trace)를 map에 추가
		//pAI->SetCurState(MON_STATE::IDLE);	// AI 상태를 IDLE로 설정
		//pMon->SetAI(pAI);
	}
		break;
	case MON_TYPE::RANGE:


		break;
	}

	assert(pMon);	// nullptr에 걸린다는 것은 case 어디에도 안 걸렸다는 뜻임
	return pMon;
}

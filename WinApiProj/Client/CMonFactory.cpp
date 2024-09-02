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
		pMon = new CMonster;	// ���� ����
		pMon->SetPos(_vPos);	// ���� ��ġ

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.fRecogRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = 150.f;

		pMon->SetMonInfo(info);

		pMon->CreateRigidBody();
		pMon->GetRigidBody()->SetMass(1.f);

		//AI* pAI = new AI;					// AI ����
		//pAI->AddState(new CIdleState);		// Monster AI�� ���� �� �ִ� ����(Idle)�� map�� �߰�
		//pAI->AddState(new CTraceState);		// Monster AI�� ���� �� �ִ� ����(Trace)�� map�� �߰�
		//pAI->SetCurState(MON_STATE::IDLE);	// AI ���¸� IDLE�� ����
		//pMon->SetAI(pAI);
	}
		break;
	case MON_TYPE::RANGE:


		break;
	}

	assert(pMon);	// nullptr�� �ɸ��ٴ� ���� case ��𿡵� �� �ɷȴٴ� ����
	return pMon;
}

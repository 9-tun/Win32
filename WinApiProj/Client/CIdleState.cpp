#include "pch.h"
#include "CIdleState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

// �θ� �⺻ �����ڰ� ���µ� �ڽ��� �⺻ �����ڷ� �θ�, �ڽ��� �θ��� �⺻ �����ڸ� ȣ���ϸ鼭 ���� �߻�
CIdleState::CIdleState()
	: CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::update()
{
	// Player ��ġ üũ
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();	
	Vec2 vPlayerPos = pPlayer->GetPos();

	// ���� ���� �ȿ� ������ ���� ���·� ��ȯ
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	// �÷��̾ ������ �νĹ��� ������ ����
	if (fLen < pMonster->GetInfo().fRecogRange)
	{
		// Monster�� ���� �� ����
		// ������ �̰͵� ������ update �߰��� �ٲ�� ��ü���� Monster�� ���¸� �ٸ��� �ν��� �� �����ϱ� �̹��� ��û�� ���� ������ �ٲ�� �̺�Ʈ ó������� ��. func�� �̺�Ʈ ����
		// �̺�Ʈ �Ŵ����� ó���ϵ��� �̺�Ʈ�� ��Ƶξ��ٰ� �̸� ���߿� �̺�Ʈ �Ŵ����� update�� ó���ϵ��� ����
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}
}

void CIdleState::Enter()
{

}

void CIdleState::Exit()
{
}

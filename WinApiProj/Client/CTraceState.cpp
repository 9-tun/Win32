#include "pch.h"
#include "CTraceState.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CPlayer.h"
#include "CMonster.h"

#include "CtimeMgr.h"

CTraceState::CTraceState()
	: CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}
void CTraceState::update()
{
	// Å¸°ÙÆÃµÈ °´Ã¼¸¦ ÂÑ¾Æ°£´Ù.
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();
	Vec2 vMonPos = GetMonster()->GetPos();

	Vec2 vMonDir = vPlayerPos - vMonPos;
	vMonDir.Nomalize();

	vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;
	GetMonster()->SetPos(vMonPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}



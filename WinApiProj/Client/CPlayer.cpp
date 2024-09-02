#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CKeyMgr.h"
#include "CtimeMgr.h"

#include "CMissile.h"

#include "CResMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CRigidBody.h"
#include "CGravity.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::WALK)
	, m_iDir(1)
	, m_iPrevDir(1)
{
	// Texture 로딩하기
	//m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\Player.bmp");
	CreateCollider();	// Player는 충돌 기능을 가질 것이기 때문에 생성자에서 부모 Object의 CreateCollider 함수를 호출
	GetCollider()->SetOffsetPos(Vec2(0.f, 20.f));	// 충돌체 OffSet
	GetCollider()->SetScale(Vec2(20.f, 20.f));		// 충돌체 크기

	CreateRigidBody();	// CObject의 함수 호출

	// Texture 로딩하기
	CTexture* pLeftTex = CResMgr::GetInst()->LoadTexture(L"PlayerLeft", L"texture\\Player01_L.bmp");
	CTexture* pRightTex = CResMgr::GetInst()->LoadTexture(L"PlayerRight", L"texture\\Player01_R.bmp");

	CreateAnimator();

	GetAnimator()->LoadAnimation(L"animation\\player_idle_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_idle_right.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_walk_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_walk_right.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_jump_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_jump_right.anim");

	//GetAnimator()->CreateAnimation(L"IDLE_LEFT", pLeftTex, Vec2(0.f, 0.f), Vec2(88.f, 88.f), Vec2(88.f, 0.f), 0.05f, 9);
	//GetAnimator()->CreateAnimation(L"IDLE_RIGHT", pRightTex, Vec2(0.f, 0.f), Vec2(88.f, 88.f), Vec2(88.f, 0.f), 0.05f, 9);

	//GetAnimator()->CreateAnimation(L"WALK_LEFT", pLeftTex, Vec2(0.f, 88.f), Vec2(88.f, 88.f), Vec2(88.f, 0.f), 0.05f, 4);
	//GetAnimator()->CreateAnimation(L"WALK_RIGHT", pRightTex, Vec2(440.f,88.f), Vec2(88.f, 88.f), Vec2(88.f, 0.f), 0.05f, 4);

	//GetAnimator()->CreateAnimation(L"JUMP_LEFT", pLeftTex, Vec2(0.f, 176.f), Vec2(88.f, 88.f), Vec2(88.f, 0.f), 0.05f, 1);
	//GetAnimator()->CreateAnimation(L"JUMP_RIGHT", pRightTex, Vec2(704.f, 176.f), Vec2(88.f, 88.f), Vec2(88.f, 0.f), 0.05f, 1);

	//// Animation 저장해 보기
	//GetAnimator()->FindAnimation(L"IDLE_LEFT")->Save(L"animation\\player_idle_left.anim");
	//GetAnimator()->FindAnimation(L"IDLE_RIGHT")->Save(L"animation\\player_idle_right.anim");
	//GetAnimator()->FindAnimation(L"WALK_LEFT")->Save(L"animation\\player_walk_left.anim");
	//GetAnimator()->FindAnimation(L"WALK_RIGHT")->Save(L"animation\\player_walk_right.anim");
	//GetAnimator()->FindAnimation(L"JUMP_LEFT")->Save(L"animation\\player_jump_left.anim");
	//GetAnimator()->FindAnimation(L"JUMP_RIGHT")->Save(L"animation\\player_jump_right.anim");

	CreateGravity();
}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	update_move();
	update_state();

	update_animation();	// 상태 변경이 감지되면 animation을 update

	if (KEY_TAP(KEY::ENTER))
	{
		SetPos(Vec2(640.f, 384.f));
	}

	GetAnimator()->update();

	m_ePrevState = m_eCurState;	// 매번 끝날 때 현재 상태를 이전 상태로 받기
	m_iPrevDir = m_iDir;
}

void CPlayer::render(HDC _dc)
{
	/*int iWidth = (int)m_pTex->Width();
	int iHeight = (int)m_pTex->Height();

	Vec2 vPos = GetPos();*/

	/*BitBlt(_dc
		, int(vPos.x - (float)(iWidth / 2))
		, int(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, SRCCOPY);*/

	// 비트맵 복사해 올 때 특정 RGB 색상(배경색)은 제외하고 복사
	/*TransparentBlt(_dc
		, int(vPos.x - (float)(iWidth / 2))
		, int(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth, iHeight
		, RGB(255, 0, 255));*/

	// 컴포넌트 (충돌체, etc)가 있는 경우 렌더
	component_render(_dc);	// 애니매이션을 구현하면서 player 객체는 Animation render로 표시됨

	// --------------------------------------------------------------------------------------------
	// 알파 블렌드 테스트 코드
	// 위 component_render(_dc) 주석 처리하고 이거 주석 해제하면 알파 블렌드 비행기 뜸

	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Plane", L"texture\\Player_01.bmp");

	//Vec2 vPos = GetPos();
	//vPos = CCamera::GetInst()->GetRenderPos(vPos);

	//float width = (float)pTex->Width();
	//float height = (float)pTex->Height();

	//BLENDFUNCTION bf = {};
	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.SourceConstantAlpha = 127;

	//AlphaBlend(_dc
	//	, int(vPos.x - width/2.f	 )
	//	, int(vPos.y - height/2.f	 )
	//	, int(width), int(height)
	//	, pTex->GetDC()
	//	, 0, 0, int(width), int(height)
	//	, bf);
	// --------------------------------------------------------------------------------------------
}

// CPlayer 위치 Update할 때 Key 입력 값에 Space가 들어오면 호출됨
void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	// Missile Object
	CMissile* pMissile = new CMissile;
	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(Vec2(0.f, -1.f));

	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);

}

// 캐릭터 상태를 키 입력 뿐 아니라 공격을 받는 등의 상태도 모두 여기서 관리
void CPlayer::update_state()
{
	if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;		// 왼쪽이 눌렸을 때, m_iDIr = -1로 세팅
		if(PLAYER_STATE::JUMP != m_eCurState)
			m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;			// 오른쪽이 눌렸을 때, m_iDIr = 1로 세팅
		if (PLAYER_STATE::JUMP != m_eCurState)
			m_eCurState = PLAYER_STATE::WALK;
	}

	if (0.f == GetRigidBody()->GetSpeed() && PLAYER_STATE::JUMP != m_eCurState)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}

	/*if (KEY_TAP(KEY::SPACE))
	{
		CreateMissile();
	}*/

	if (KEY_TAP(KEY::SPACE))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		if (GetRigidBody())
		{
			GetRigidBody()->SetVelocity(Vec2(GetRigidBody()->GetVelocity().x,-300.f));
		}
	}



}

void CPlayer::update_move()
{
	CRigidBody* pRigid = GetRigidBody();

	if (KEY_HOLD(KEY::A))
	{
		pRigid->AddForce(Vec2(-200.f, 0.f));
	}
	if (KEY_HOLD(KEY::D))
	{
		pRigid->AddForce(Vec2(200.f, 0.f));
	}

	if (KEY_TAP(KEY::A))
	{	
		pRigid->SetVelocity(Vec2(-100.f, pRigid->GetVelocity().y));
	}
	if (KEY_TAP(KEY::D))
	{
		pRigid->SetVelocity(Vec2(100.f, pRigid->GetVelocity().y));
	}
}

void CPlayer::update_animation()
{
	if (m_ePrevState == m_eCurState && m_iPrevDir == m_iDir)
		return;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		if(m_iDir == -1)
			GetAnimator()->Play(L"IDLE_LEFT", true);
		else
			GetAnimator()->Play(L"IDLE_RIGHT", true);
	}
		break;
	case PLAYER_STATE::WALK:
	{
		if (m_iDir == -1)
			GetAnimator()->Play(L"WALK_LEFT", true);
		else
			GetAnimator()->Play(L"WALK_RIGHT", true);
	}
		break;
	case PLAYER_STATE::JUMP:
	{
		if (m_iDir == -1)
			GetAnimator()->Play(L"JUMP_LEFT", true);
		else
			GetAnimator()->Play(L"JUMP_RIGHT", true);
	}
		break;
	case PLAYER_STATE::ATTACK:

		break;
	case PLAYER_STATE::DEAD:

		break;
	}
		
}

void CPlayer::update_gravity()
{
	GetRigidBody()->AddForce(Vec2(0.f, 500.f));
}

void CPlayer::OnCollisionEnter(CCollider * _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (L"Ground" == pOtherObj->GetName())
	{
		Vec2 vPos = GetPos();
		if (vPos.y < pOtherObj->GetPos().y)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}
	}
}



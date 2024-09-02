#include "pch.h"
#include "CScene_Start.h"

#include "CObject.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCore.h"

#include "CTexture.h"
#include "CPathMgr.h"

#include "CCollisionMgr.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"

#include "AI.h"
#include "CIdleState.h"
#include "CTraceState.h"

#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CtimeMgr.h"

#include "CGround.h"

CScene_Start::CScene_Start()
	: m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
{
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::update()
{
	if (KEY_HOLD(KEY::LBTN))
	{
		m_bUseForce = true;
		CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}

	// CScene Start에서는 부모 CScene에서 update를 호출하지 않고 여기에 직접 구현함
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)i);		

		for (UINT j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->IsDead())	// 객체가 죽은 상태가 아니라면
			{
				if (m_bUseForce && vecObj[j]->GetRigidBody())
				{
					Vec2 vDiff = vecObj[j]->GetPos() - m_vForcePos ;		// 객체와 힘 발생지 간의 Vector
					float fLen = vDiff.Length();						// 객체와 힘 발생지 간의 거리
					if (fLen < m_fForceRadius)							
					{
						float fRatio = 1.f - (fLen / m_fForceRadius);	// 힘 발생지에서 가까울수록 힘을 온전히 다 받음
						float fForce = m_fForce * fRatio;				// 실제 적용될 최종 힘

						vecObj[j]->GetRigidBody()->AddForce(vDiff.Nomalize()*fForce);
					}
				}

				vecObj[j]->update();	// CObject* 로 Scene에 표시되는 Obj의 위치 값 업데이트 
			}
		}
	}

	//if (KEY_TAP(KEY::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::TOOL);	// 전역 함수로 씬 변경 이벤트 등록
	//}

	// 클릭한 좌표로 카메라 이동하는 코드
	//if (KEY_TAP(KEY::LBTN))
	//{
	//	Vec2 vLookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
	//	CCamera::GetInst()->SetLookAt(vLookAt);
	//}
}

void CScene_Start::render(HDC _dc)
{
	CScene::render(_dc);		// 부모 Scene 에서 객체를 모두 그린 후에, 자식 scene에서 특별히 더 그려주고 싶은 것은 추가로 그리기

	SelectGDI gdi1(_dc, BRUSH_TYPE::HOLOW);
	SelectGDI gdi2(_dc, PEN_TYPE::GREEN);

	if (!m_bUseForce)
		return;

	m_fCurRadius += m_fForceRadius * 3.f * fDT;
	if (m_fCurRadius > m_fForceRadius)
	{
		m_fCurRadius = 0.f;
	}

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);	

	Ellipse(_dc
		, (int)(vRenderPos.x - m_fCurRadius)
		, (int)(vRenderPos.y - m_fCurRadius)
		, (int)(vRenderPos.x + m_fCurRadius)
		, (int)(vRenderPos.y + m_fCurRadius));
	
}

void CScene_Start::Enter()
{
	// Object 추가
	CObject* pObj = new CPlayer;
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	RegisterPlayer(pObj);	// Player 생성 후 이를 등록, Player 찾아 쓰기 쉽게

	/*CObject* pOtherPlayer = pObj->Clone();
	pOtherPlayer->SetPos(Vec2(840.f, 384.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	// 화면이 pObj(플레이어)를 따라다님
	//CCamera::GetInst()->SetTarget(pObj);

	// 몬스터 배치
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	//CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	//AddObject(pMon, GROUP_TYPE::MONTSTER);		// Scene에 처음 들어가니까 object를 더해주지면 씬 진입 이후에는 CreateObject 이벤트로 Monster를 생성하자. 그럼 이벤트 매니저 업데이트할 때 모아놨던 거 한번에 실행

	// 땅 물체 배치
	CGround* pGround = new CGround;
	pGround->SetName(L"Ground");
	pGround->SetPos(Vec2(640.f, 584.f));
	pGround->SetScale(Vec2(500.f, 60.f));
	AddObject(pGround, GROUP_TYPE::GROUND);

	// 타일 로딩
	//LoadTile(L"Tile\\Start.tile");

	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌 체크
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONTSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONTSTER, GROUP_TYPE::PROJ_PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);

	// Camera Look 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	// Camera 효과 지정
	CCamera::GetInst()->FadeOut(1.f);
	CCamera::GetInst()->FadeIn(1.f);

	start();
}

void CScene_Start::Exit()
{
	DeleteAll();
	// 다른 씬으로 가면 충돌 그룹이 달라질 수 있으니 설정을 해제해 줘야 한다.
	CCollisionMgr::GetInst()->Reset();
}

void CScene_Start::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
}

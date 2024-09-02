#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}


void CCollisionMgr::update()
{
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		m_arrCheck[iRow];
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);	// 더 작은 수가 왼쪽 매개인자에 있음
			}
		}
	}
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로, 
	// 큰 값을 열(비트)로 사용
	// 매트릭스 좌상단 절반
	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	// 행(iRow)과 열(iCol)의 값이 1이면 0으로, 0이면 1로 바꾸는 토글 

	if (m_arrCheck[iRow] & (1 << iCol))		// 해당 비트가 1인지 확인합니다.
	{
		m_arrCheck[iRow] &= ~(1 << iCol);	// 해당 비트를 0으로 만듭니다.
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);	// 해당 비트를 1로 만듭니다.
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);	// 자료형을 그냥 vector<CObject*>로 받으면 지역변수이지 참조변수가 아님, 참조 받아온 걸 지역변수로 복사 받는 형태
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);	// const vector<CObject*>& 로 받아야 참조변수가 되고, 복사 비용을 줄일 수 있음
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 충돌체를 보유하지 않은 경우
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}


		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나 자기 자신과 충돌인 경우
			if (nullptr == vecRight[j]->GetCollider() || vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);	// ID.ID에 해당하는 키 값을 찾아서 해당 요소를 iterator가 가리킴

			// 충돌 정보가 미 등록 상태인 경우 등록 (충돌하지 않았다로)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}


			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌 중이다
				if (true == iter->second )
				{
					// 이전에도 충돌하고 있었다.
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())	// 하지만 그 객체가 다음 프레임에서 죽어 없어질 객체일 경우
					{
						// 근데 둘 중 하나가 삭제 예정이라면, 충돌 해제를 해준다.
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
					
				}
				else 
				{
					// 이전에는 충돌하지 않았다.
					// 공교롭게도 곧 삭제될 객체가 이전에 충돌하지 않다가 이번에 충돌하게 된 경우
					// 근데 둘 중 하나가 삭제 예정이라면, 충돌하지 않은 것으로 취급
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())	
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}					
				}
			}
			else
			{
				// 현재 충돌하고 있지 않다.
				if (iter->second)
				{
					// 이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;					
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}
	return false;
}

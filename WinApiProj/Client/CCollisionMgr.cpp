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
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);	// �� ���� ���� ���� �Ű����ڿ� ����
			}
		}
	}
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// �� ���� ���� �׷� Ÿ���� ������, 
	// ū ���� ��(��Ʈ)�� ���
	// ��Ʈ���� �»�� ����
	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	// ��(iRow)�� ��(iCol)�� ���� 1�̸� 0����, 0�̸� 1�� �ٲٴ� ��� 

	if (m_arrCheck[iRow] & (1 << iCol))		// �ش� ��Ʈ�� 1���� Ȯ���մϴ�.
	{
		m_arrCheck[iRow] &= ~(1 << iCol);	// �ش� ��Ʈ�� 0���� ����ϴ�.
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);	// �ش� ��Ʈ�� 1�� ����ϴ�.
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);	// �ڷ����� �׳� vector<CObject*>�� ������ ������������ ���������� �ƴ�, ���� �޾ƿ� �� ���������� ���� �޴� ����
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);	// const vector<CObject*>& �� �޾ƾ� ���������� �ǰ�, ���� ����� ���� �� ����
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// �浹ü�� �������� ���� ���
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}


		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// �浹ü�� ���ų� �ڱ� �ڽŰ� �浹�� ���
			if (nullptr == vecRight[j]->GetCollider() || vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);	// ID.ID�� �ش��ϴ� Ű ���� ã�Ƽ� �ش� ��Ҹ� iterator�� ����Ŵ

			// �浹 ������ �� ��� ������ ��� ��� (�浹���� �ʾҴٷ�)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}


			if (IsCollision(pLeftCol, pRightCol))
			{
				// ���� �浹 ���̴�
				if (true == iter->second )
				{
					// �������� �浹�ϰ� �־���.
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())	// ������ �� ��ü�� ���� �����ӿ��� �׾� ������ ��ü�� ���
					{
						// �ٵ� �� �� �ϳ��� ���� �����̶��, �浹 ������ ���ش�.
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
					// �������� �浹���� �ʾҴ�.
					// �����ӰԵ� �� ������ ��ü�� ������ �浹���� �ʴٰ� �̹��� �浹�ϰ� �� ���
					// �ٵ� �� �� �ϳ��� ���� �����̶��, �浹���� ���� ������ ���
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
				// ���� �浹�ϰ� ���� �ʴ�.
				if (iter->second)
				{
					// �������� �浹�ϰ� �־���.
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

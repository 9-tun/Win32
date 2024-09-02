#pragma once

class CCollider;

// union을 사용하면 64비트의 ID를 Left_id와 Right_id로 쪼개서 관리할 수 있고 반대도 가능하다.
union COLLIDER_ID
{
	struct{
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr);

private:
	// 충돌체 간의 이전 프레임 충돌 정보
	map<ULONGLONG, bool>	m_mapColInfo;						// 충돌체 간의 이전 프레임 충돌 정보, 키: 두 충돌체 ID로 만든 정보, 값: 두 충돌체의 이전 상태 정보
	UINT					m_arrCheck[(UINT)GROUP_TYPE::END];	// 그룹 간 충돌 체크 매트릭스

public:
	void update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END); }

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

};


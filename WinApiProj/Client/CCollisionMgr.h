#pragma once

class CCollider;

// union�� ����ϸ� 64��Ʈ�� ID�� Left_id�� Right_id�� �ɰ��� ������ �� �ְ� �ݴ뵵 �����ϴ�.
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
	// �浹ü ���� ���� ������ �浹 ����
	map<ULONGLONG, bool>	m_mapColInfo;						// �浹ü ���� ���� ������ �浹 ����, Ű: �� �浹ü ID�� ���� ����, ��: �� �浹ü�� ���� ���� ����
	UINT					m_arrCheck[(UINT)GROUP_TYPE::END];	// �׷� �� �浹 üũ ��Ʈ����

public:
	void update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	void Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END); }

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

};


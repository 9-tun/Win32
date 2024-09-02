#pragma once

class CObject;

class CCollider
{
private:

	static UINT g_iNextID;		// ��������� ��ü�� ���� �� ��

	CObject*	m_pOwner;		// Collider�� �����ϰ� �ִ� ������Ʈ
	Vec2		m_vOffsetPos;	// ������Ʈ�κ��� ������� ��ġ
	Vec2		m_vFinalPos;	// finalupdate���� �� �����Ӹ��� ���
	Vec2		m_vScale;		// �浹ü ũ�� 

	UINT		m_iID;			// �浹ü ������ ID��
	int			m_iCol;			// �浹 ī����

	bool		m_bActive;		// �浹ü Ȱ��ȭ ����

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }

	UINT GetID() { return m_iID; }

public:
	void finalupdate();
	void render(HDC _dc);

public:
	// �浹 ���� �Լ�
	void OnCollision(CCollider* _pOther);		// �浹 ���� ��� ȣ��Ǵ� �Լ� 
	void OnCollisionEnter(CCollider* _pOther);	// �浹 ���� �� = ù �浹 �߻� �� ȣ��Ǵ� �Լ�
	void OnCollisionExit(CCollider* _pOther);	// �浹 ���� �� = �浹���� �� ��� ����

	// �浹ü�� ���� �����ڸ� ������ �ʿ䰡 ������, ���Ƶ���
	// = delete Ű����� �ش� �Լ� �Ǵ� �����ڸ� ����� �� ���� �����.
	CCollider& operator= (CCollider& _origin) = delete;	// ���� �̷��� ���� �������� �ʾ����� �⺻ ���Կ����ڰ� ��������� ������ �ǰ� �ȴ�.

public:
	CCollider();
	// ���� ���� ������ (�ֳĸ� m_iID�� �浹ü���� �����ؾ� �ϱ� ����)
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};


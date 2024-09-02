#pragma once

#include "global.h"
#include "CCamera.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;

class CObject
{
private:
	wstring		m_strName;

	Vec2		m_vPos;
	Vec2		m_vScale;

	// Component�� ������ ����: Object�� �浹ü ������ ���� ��� ��ӱ����� �� ������ �ȴ�. (����: �� �ϴٺ��� �浹�� �����ٰ� ���߿� ���ܰ� ����)
	CCollider*	m_pCollider;	// ��ü �浹�� �ʿ��ϸ� CCollider �����͸� ����, �ʿ� ������ nullptr�� ������ �ȴ�. 
	CAnimator*	m_pAnimator;
	CRigidBody* m_pRigidBody;
	CGravity*	m_pGravity;		// �̷��� Component�� ������ �� �������� Component�� �θ� Class�� ���� ������ �ϳ��� ����Ű�� �߾ �ƾ���. (������)

	bool		m_bAlive;

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vSacle) { m_vScale = _vSacle; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	CGravity* GetGravity() { return m_pGravity; }

	bool IsDead() { return !m_bAlive; }

	void CreateCollider();	// �浹�� �ʿ��� ��ü�� ��ü������ �Լ��� ȣ���ؼ� ���´�.
	void CreateAnimator();	// Animation�� �ʿ��� ��ü�� ��ü������ �Լ� ȣ��
	void CreateRigidBody();
	void CreateGravity();

	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

private:
	void SetDead() { m_bAlive = false; }	// Object ���̴� ���� �ڽ� Ŭ���� �������� ���ϰ�, ���� CEventMgr�θ� ó���ؾ� �Ѵ�. �Ʒ� friend class CEventMgr�� ó��

public:
	virtual void start() {};	// Scene�� ���۵Ǳ� ������ ȣ��Ǵ� �Լ�, �ʿ��� Object���� �������̵��ؼ� ���

	// �� Object ���� ������Ʈ�� �������� �޶�� �ؼ� �����Լ��� ����
	virtual void update() = 0;	// =0�� �ٿ��� ���������Լ��� ���� CObject�� ��ӹ��� Ŭ�������� ������ ���� �ڱ���� update�� �����ϵ��� ����
	//virtual void finalupdate() final;	// �������� final�� �ٿ��� �� �̻� �ڽ��� �ش� �Լ��� �������̵����� ���ϵ��� ����, CCollider�� ��ü������ ��ü�� ����ٴϵ��� ��
	virtual void finalupdate();			// UI���� finalupdate�� �ʿ��ؼ� ������ final�� ������, UI�� ������ ������ Object���� finalupdate�� �������̵��ؼ� �������� ����
	virtual void render(HDC _dc);

	void component_render(HDC _dc);

	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);		// ���� ������ (m_pCollider�� ������ �ּ� �����ε� �̰ű��� �״�� �����ϸ� ������ ��, ���� ���� ����, m_bAlive�� ����)
	virtual ~CObject();

	friend class CEventMgr;					// �̺�Ʈ �Ŵ��������� Object�� ���� �� �ִ�.
};


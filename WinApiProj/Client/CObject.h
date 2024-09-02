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

	// Component로 구현한 이유: Object를 충돌체 유무에 따라 모두 상속구조로 안 나눠도 된다. (이유: 꼭 하다보면 충돌로 나눴다가 나중에 예외가 생김)
	CCollider*	m_pCollider;	// 객체 충돌이 필요하면 CCollider 포인터를 갖고, 필요 없으면 nullptr을 가지면 된다. 
	CAnimator*	m_pAnimator;
	CRigidBody* m_pRigidBody;
	CGravity*	m_pGravity;		// 이렇게 Component가 많아질 것 같았으면 Component의 부모 Class를 만들어서 포인터 하나로 가리키게 했어도 됐었다. (다형성)

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

	void CreateCollider();	// 충돌이 필요한 객체는 자체적으로 함수를 호출해서 갖는다.
	void CreateAnimator();	// Animation이 필요한 객체는 자체적으로 함수 호출
	void CreateRigidBody();
	void CreateGravity();

	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

private:
	void SetDead() { m_bAlive = false; }	// Object 죽이는 것은 자식 클래스 조차하지 못하고, 오직 CEventMgr로만 처리해야 한다. 아래 friend class CEventMgr로 처리

public:
	virtual void start() {};	// Scene이 시작되기 직전에 호출되는 함수, 필요한 Object들은 오버라이딩해서 사용

	// 각 Object 별로 업데이트와 렌더링이 달라야 해서 가상함수로 선언
	virtual void update() = 0;	// =0을 붙여서 순수가상함수로 만들어서 CObject를 상속받은 클래스들은 무조건 각자 자기들의 update를 구현하도록 설정
	//virtual void finalupdate() final;	// 마지막에 final을 붙여서 더 이상 자식이 해당 함수를 오버라이딩하지 못하도록 막음, CCollider가 자체적으로 객체를 따라다니도록 함
	virtual void finalupdate();			// UI에서 finalupdate가 필요해서 마지막 final을 해제함, UI를 제외한 나머지 Object들은 finalupdate를 오버라이딩해서 구현하지 않음
	virtual void render(HDC _dc);

	void component_render(HDC _dc);

	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);		// 복사 생성자 (m_pCollider는 포인터 주소 형식인데 이거까지 그대로 복사하면 문제가 됨, 따라서 별도 구현, m_bAlive도 문제)
	virtual ~CObject();

	friend class CEventMgr;					// 이벤트 매니저에서만 Object를 죽일 수 있다.
};


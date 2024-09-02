#pragma once

class CObject;

class CCollider
{
private:

	static UINT g_iNextID;		// 정적멤버는 객체에 포함 안 됨

	CObject*	m_pOwner;		// Collider를 소유하고 있는 오브젝트
	Vec2		m_vOffsetPos;	// 오브젝트로부터 상대적인 위치
	Vec2		m_vFinalPos;	// finalupdate에서 매 프레임마다 계산
	Vec2		m_vScale;		// 충돌체 크기 

	UINT		m_iID;			// 충돌체 고유한 ID값
	int			m_iCol;			// 충돌 카운터

	bool		m_bActive;		// 충돌체 활성화 여부

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
	// 충돌 시점 함수
	void OnCollision(CCollider* _pOther);		// 충돌 중인 경우 호출되는 함수 
	void OnCollisionEnter(CCollider* _pOther);	// 충돌 진입 시 = 첫 충돌 발생 시 호출되는 함수
	void OnCollisionExit(CCollider* _pOther);	// 충돌 해제 시 = 충돌에서 막 벗어난 시점

	// 충돌체는 대입 연산자를 정의할 필요가 없으니, 막아두자
	// = delete 키워드는 해당 함수 또는 연산자를 사용할 수 없게 만든다.
	CCollider& operator= (CCollider& _origin) = delete;	// 만약 이렇게 따로 정의하지 않았으면 기본 대입연산자가 만들어져서 대입이 되게 된다.

public:
	CCollider();
	// 깊은 복사 생성자 (왜냐면 m_iID는 충돌체마다 고유해야 하기 때문)
	CCollider(const CCollider& _origin);
	~CCollider();

	friend class CObject;
};


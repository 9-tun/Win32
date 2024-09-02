#pragma once
#include "CObject.h"

// Object가 Ground와 충돌이 있을 때, Object에서 감지해서 구현해도 되고, Ground에서 감지해서 Object에 보내줘도 된다.
// 하지만 객체에서 구현하면 모든 객체마다 감지하는 동작이 필요하므로 Gound에서 감지해서 각 Object로 충돌 정보를 보내주는 게 편리하다.

class CGround :
	public CObject
{
private:
	virtual void start();
	virtual void update();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	CLONE(CGround);

public:
	CGround();
	~CGround();
};


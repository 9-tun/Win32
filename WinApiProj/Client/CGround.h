#pragma once
#include "CObject.h"

// Object�� Ground�� �浹�� ���� ��, Object���� �����ؼ� �����ص� �ǰ�, Ground���� �����ؼ� Object�� �����൵ �ȴ�.
// ������ ��ü���� �����ϸ� ��� ��ü���� �����ϴ� ������ �ʿ��ϹǷ� Gound���� �����ؼ� �� Object�� �浹 ������ �����ִ� �� ���ϴ�.

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


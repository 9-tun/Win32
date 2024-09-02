#pragma once
#include "CObject.h"


class CMissile :
	public CObject
{
private:
	float m_fTheta;	// �̵� ����
	Vec2 m_vDir;	// ���� ����

public:
	void SetDir(float _fTheda) { m_fTheta = _fTheda; }
	void SetDir(Vec2 _vDir) 
	{ 
		m_vDir = _vDir; 		
		m_vDir.Nomalize();
	}

public:
	virtual void update();
	virtual void render(HDC _dc);

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	CLONE(CMissile);
	
public:
	CMissile();
	~CMissile();

};


#pragma once
#include "CObject.h"

class CTexture;

class CTile :
	public CObject
{
private:
	CTexture*	m_pTileTex;
	int			m_iImgIdx;			// int형으로 선언한 이유는 m_iImgIdx가 -1일 경우 아무 것도 참조하지 않겠다는 의미로 사용하려고 함

public:
	void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }
	void AddImgIdx() { ++m_iImgIdx; }

private:
	virtual void update();
	virtual void render(HDC _dc);

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	
	CLONE(CTile);

public:
	CTile();
	~CTile();
};


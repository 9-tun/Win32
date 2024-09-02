#pragma once

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim;		// 모든 Animation
	CAnimation*					m_pCurAnim;		// 현재 재생 중인 Animation
	CObject*					m_pOwner;		// Animator 소유 오브젝트
	bool						m_bRepeat;		// 반복 재생 여부

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _pStrName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);	// 애니매이션 생성
	void LoadAnimation(const wstring& _strRelativePath);
	CAnimation* FindAnimation(const wstring& _strName);	// 애니매이션 찾기
	void Play(const wstring& _strName, bool _bRepeat);			// 애니매이션 재생

	void update();
	void finalupdate();
	void render(HDC _dc);

public:
	CAnimator();
	~CAnimator();
	
	friend class CObject;
};


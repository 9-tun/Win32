#pragma once

#include "global.h"

#include "CMonFactory.h"

// 전방 선언
// 컴파일 속도에 영향을 주지 않기 위해서, 직접 헤더를 참조했다면 CObject 코드 변경 시, Scene 코드도 컴파일러가 다시 확인을 해야 함
// 헤더끼리 서로가 서로를 참조하는 걸 최대한 방지하고자 전방 선언을 사용
// 단, 전방 선언을 하면 실제 타입에 대한 구체적 정보가 없기 때문에 포인터 타입으로만 사용해야 함
class CObject;

class CScene
{
private:
	vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END];	// 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언, vector<Object*>를 담는 배열
	wstring			 m_strName;	// Scene 이름

	UINT			m_iTileX;	// 타일 가로 개수
	UINT			m_iTileY;	// 타일 세로 개수

	CObject*		m_pPlayer;	// Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	CObject* GetPlayer() { return m_pPlayer; }

	virtual void start();

	virtual void update();			// Scene에 들어있는 각 객체들 위치값 Update
	virtual void finalupdate();
	virtual void render(HDC _dc);

	void render_tile(HDC _dc);		// 타일용 렌더링

	// 순수 가상 함수: 부모에서는 구현할 필요가 없고, 자식에서는 꼭 구현해야 하는 인터페이스형 함수
	virtual void Enter() = 0;	// 해당 Scene에 진입 시 호출
	virtual void Exit() = 0;	// 해당 Scene에 탈출 시 호출

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	// vector<CObject*>로 반환 받으면 원본이 아닌 복사본이 반환됨
	// 그래서 vector<CObject>&로 참조값 반환, 단 변경이 없도록 const
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();
	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);

	// UI는 포커싱 잡힌 애가 마지막에 render될 수 있도록 배열 순서 조정이 필요해서 const 제거된 애로 따로 정의
	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

public:
	CScene();
	virtual ~CScene();
};


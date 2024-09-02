#pragma once

#include "global.h"

#include "CMonFactory.h"

// ���� ����
// ������ �ӵ��� ������ ���� �ʱ� ���ؼ�, ���� ����� �����ߴٸ� CObject �ڵ� ���� ��, Scene �ڵ嵵 �����Ϸ��� �ٽ� Ȯ���� �ؾ� ��
// ������� ���ΰ� ���θ� �����ϴ� �� �ִ��� �����ϰ��� ���� ������ ���
// ��, ���� ������ �ϸ� ���� Ÿ�Կ� ���� ��ü�� ������ ���� ������ ������ Ÿ�����θ� ����ؾ� ��
class CObject;

class CScene
{
private:
	vector<CObject*> m_arrObj[(UINT)GROUP_TYPE::END];	// ������Ʈ�� ���� �� ������ ���͸� �׷� ������ŭ ����, vector<Object*>�� ��� �迭
	wstring			 m_strName;	// Scene �̸�

	UINT			m_iTileX;	// Ÿ�� ���� ����
	UINT			m_iTileY;	// Ÿ�� ���� ����

	CObject*		m_pPlayer;	// Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	CObject* GetPlayer() { return m_pPlayer; }

	virtual void start();

	virtual void update();			// Scene�� ����ִ� �� ��ü�� ��ġ�� Update
	virtual void finalupdate();
	virtual void render(HDC _dc);

	void render_tile(HDC _dc);		// Ÿ�Ͽ� ������

	// ���� ���� �Լ�: �θ𿡼��� ������ �ʿ䰡 ����, �ڽĿ����� �� �����ؾ� �ϴ� �������̽��� �Լ�
	virtual void Enter() = 0;	// �ش� Scene�� ���� �� ȣ��
	virtual void Exit() = 0;	// �ش� Scene�� Ż�� �� ȣ��

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	// vector<CObject*>�� ��ȯ ������ ������ �ƴ� ���纻�� ��ȯ��
	// �׷��� vector<CObject>&�� ������ ��ȯ, �� ������ ������ const
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();
	void CreateTile(UINT _iXCount, UINT _iYCount);
	void LoadTile(const wstring& _strRelativePath);

	// UI�� ��Ŀ�� ���� �ְ� �������� render�� �� �ֵ��� �迭 ���� ������ �ʿ��ؼ� const ���ŵ� �ַ� ���� ����
	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

public:
	CScene();
	virtual ~CScene();
};


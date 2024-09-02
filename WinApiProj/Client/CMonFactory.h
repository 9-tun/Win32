#pragma once

class CMonster;

enum class MON_TYPE
{
	NORMAL,
	RANGE,
};

class CMonFactory
{
public:
	// ��ü ���� ȣ���� �� �ִ� ��� �Լ� = ���� ��� �Լ�
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);	// CMonFactory���� ��ü ���� �ٷ� ȣ�� ����

// CMonFactory�� ��ü�� ������ ���� ���̴�.
// ���� ���� Scene�� Ư�� Monster�� �߰��ϴ� ���Ҹ� ����, �׷��� �����ڿ� �Ҹ��ڸ� ����
private:
	CMonFactory() {}
	~CMonFactory() {}
};


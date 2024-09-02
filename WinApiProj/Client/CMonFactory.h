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
	// 객체 없이 호출할 수 있는 멤버 함수 = 정적 멤버 함수
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);	// CMonFactory에서 객체 없이 바로 호출 가능

// CMonFactory는 객체를 만들지 않을 것이다.
// 단지 현재 Scene에 특정 Monster를 추가하는 역할만 수행, 그래서 생성자와 소멸자를 숨김
private:
	CMonFactory() {}
	~CMonFactory() {}
};


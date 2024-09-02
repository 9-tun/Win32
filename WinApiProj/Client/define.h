#pragma once

// Singleton ��ũ��
// Ŭ������ �ν��Ͻ��� �ϳ��� �����ϵ��� ����, ���� ���� 
#define SINGLE(type) public:\
						static type* GetInst()\
						{\
						static type mgr;\
						return &mgr;\
						}\
					private:\
						type();\
						~type();

#define fDT CTimeMgr::GetInst() ->GetfDT()
#define DT CTimeMgr::GetInst() ->GetDT()

#define CLONE(type) type* Clone() {return new type (*this);}	// ���� ���� �����ڷ� ��ü �ڽ��� ������ ���� �Ҵ��Ͽ� ��ȯ

#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()

#define PI 3.1415926535f
#define TILE_SIZE 112.8

enum class GROUP_TYPE
{
	DEFAULT,	//0
	TILE,
	GROUND,
	MONTSTER,
	PLAYER,	
	PROJ_PLAYER,
	PROJ_MONSTER,
	
	UI = 31,	//UI�� ������ �� �� ���� �ٱ��� �׷����� �ؼ� �������� �ΰڴ�.
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,

	END,
};

enum class BRUSH_TYPE
{
	HOLOW,
	BLACK,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END
};

// ����ó���� ���� EVENT_TYPE ����
enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,

	END,
};

enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATT,
	RUN,
	DEAD,
};
#pragma once

// Singleton 매크로
// 클래스의 인스턴스가 하나만 존재하도록 보장, 전역 접근 
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

#define CLONE(type) type* Clone() {return new type (*this);}	// 깊은 복사 생성자로 객체 자신을 복제해 동적 할당하여 반환

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
	
	UI = 31,	//UI는 렌더링 될 때 가장 바깥에 그려져야 해서 마지막에 두겠다.
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

// 지연처리를 위한 EVENT_TYPE 정의
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
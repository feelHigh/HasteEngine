#pragma once

#define SINGLE(Type) private:\
						Type();\
						~Type();\
						friend class CSingleton<Type>;

#define DEVICE 	CDevice::GetInstance()->GetDevice()
#define CONTEXT CDevice::GetInstance()->GetContext()
#define MAX_LAYER 32

#define DT			CTimeMgr::GetInstance()->GetDeltaTime()
#define EngineDT	CTimeMgr::GetInstance()->GetEngineDeltaTime()

#define KEY_CHECK(KEY, STATE)	CKeyMgr::GetInstance()->GetKeyState(KEY) == STATE
#define KEY_TAP(KEY)			CKeyMgr::GetInstance()->GetKeyState(KEY) == KEY_STATE::TAP
#define KEY_PRESSED(KEY)		CKeyMgr::GetInstance()->GetKeyState(KEY) == KEY_STATE::PRESSED
#define KEY_RELEASED(KEY)		CKeyMgr::GetInstance()->GetKeyState(KEY) == KEY_STATE::RELEASED

#define CLONE(Type)				virtual Type* Clone() {return new Type(*this); }
#define CLONE_DISABLE(Type)		virtual Type* Clone() { return nullptr; }

#define DELETE(p) if(nullptr != p) delete p; p = nullptr;

#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)GetComponent(COMPONENT_TYPE::TYPE); }
#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return m_Owner->Type(); }

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

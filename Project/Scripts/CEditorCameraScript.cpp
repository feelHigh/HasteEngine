#include "pch.h"
#include "CEditorCameraScript.h"

CEditorCameraScript::CEditorCameraScript()
	: CScript(-1)
	, m_Speed(500.f)
{
}

CEditorCameraScript::~CEditorCameraScript()
{
}

void CEditorCameraScript::Tick()
{
	if (PROJECTION_TYPE::ORTHOGRAPHIC == Camera()->GetProjectionType())
	{
		OrthographicMove();
	}

	else if (PROJECTION_TYPE::PERSPECTIVE == Camera()->GetProjectionType())
	{
		PerspectiveMove();
	}

	if (KEY_TAP(KEY::P))
	{
		if (PROJECTION_TYPE::ORTHOGRAPHIC == Camera()->GetProjectionType())
			Camera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
		else
			Camera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
	}
}

void CEditorCameraScript::OrthographicMove()
{
	float Speed = m_Speed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		Speed *= 3.f;
	}

	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	Vec3 vPos = Transform()->GetRelativePosition();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += EngineDT * Speed;
	}

	Transform()->SetRelativePosition(vPos);

	float Scale = Camera()->GetScale();
	if (KEY_PRESSED(KEY::_8))
		Scale += EngineDT;

	if (KEY_PRESSED(KEY::_9))
		Scale -= EngineDT;

	Camera()->SetScale(Scale);
}

void CEditorCameraScript::PerspectiveMove()
{
	float Speed = m_Speed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		Speed *= 3.f;
	}

	Vec3 vFront = Transform()->GetWorldDirection(DIRECTION_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);

	Vec3 vPos = Transform()->GetRelativePosition();

	if (KEY_PRESSED(KEY::W))
	{
		vPos += vFront * EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos -= vFront * EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos -= vRight * EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += vRight * EngineDT * Speed;
	}

	Transform()->SetRelativePosition(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		CKeyMgr::GetInstance()->MouseCapture(true);

		// 마우스가 이동하는 방향
		//vDir.x; ==> y축 회전;
		//vDir.y; ==> x축 회전
		Vec2 vDir = CKeyMgr::GetInstance()->GetDragDirection();

		Vec3 vRot = Transform()->GetRelativeRoatation();
		vRot.y += vDir.x * XM_PI * EngineDT * 10.f;
		vRot.x += vDir.y * XM_PI * EngineDT * 10.f;
		Transform()->SetRelativeRotation(vRot);
	}
	else if (KEY_RELEASED(KEY::RBTN))
	{
		CKeyMgr::GetInstance()->MouseCapture(false);
	}
}

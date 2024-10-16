#include "pch.h"
#include "CEngine.h"

// Device Headers
#include "CDevice.h"

// Manager Headers
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CTaskMgr.h"
#include "CFontMgr.h"

// Asset Headers
#include "CPrefab.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ptResolution{}
{
}

CEngine::~CEngine()
{
	if (nullptr != m_FMODSystem)
	{
		m_FMODSystem->release();
		m_FMODSystem = nullptr;
	}
}

int CEngine::Initialize(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _Func)
{
	m_hWnd = _wnd;
	m_ptResolution = _ptResolution;
	ChangeWindowScale(_ptResolution.x, _ptResolution.y);

	if (FAILED(CDevice::GetInstance()->Initialize(m_hWnd, m_ptResolution.x, m_ptResolution.y)))
	{
		MessageBox(nullptr, L"Device initialization failed.", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// FMOD 초기화		
	FMOD::System_Create(&m_FMODSystem);
	assert(m_FMODSystem);

	// 32개 채널 생성
	m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);

	// Manager 초기화
	CPathMgr::GetInstance()->Initialize();
	CKeyMgr::GetInstance()->Initialize();
	CTimeMgr::GetInstance()->Initialize();
	CAssetMgr::GetInstance()->Initialize();
	CRenderMgr::GetInstance()->Initialize();
	CLevelMgr::GetInstance()->Initialize();
	CFontMgr::GetInstance()->Initialize();


	// Prefab Function 등록
	CPrefab::g_ObjectSaveFunc = _SaveFunc;
	CPrefab::g_ObjectLoadFunc = _Func;

	return S_OK;
}

void CEngine::Progress()
{
	// FMOD Tick
	m_FMODSystem->update();

	// Manager
	CKeyMgr::GetInstance()->Tick();
	CTimeMgr::GetInstance()->Tick();
	CAssetMgr::GetInstance()->Tick();
	CLevelMgr::GetInstance()->Progress();

	// Collision
	CCollisionMgr::GetInstance()->Tick();

	// Render	
	CRenderMgr::GetInstance()->Tick();

	// TaskMgr
	CTaskMgr::GetInstance()->Tick();
}

void CEngine::ChangeWindowScale(UINT _Width, UINT _Height)
{
	bool bMenu = false;
	if (GetMenu(m_hWnd))
		bMenu = true;

	RECT rt = { 0, 0, (int)_Width, (int)_Height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, bMenu);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}
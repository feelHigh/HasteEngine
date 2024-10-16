#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CEditorCameraScript.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"
#include "CScriptMgr.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CEditorCameraScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CScriptMgr");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CEditorCameraScript" == _strScriptName)
		return new CEditorCameraScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CScriptMgr" == _strScriptName)
		return new CScriptMgr;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::EDITORCAMERASCRIPT:
		return new CEditorCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SCRIPTMGR:
		return new CScriptMgr;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::EDITORCAMERASCRIPT:
		return L"CEditorCameraScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SCRIPTMGR:
		return L"CScriptMgr";
		break;

	}
	return nullptr;
}